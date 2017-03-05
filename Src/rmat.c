#include "rmat.h"

#define NRAND(ne) (5 * SCALE * (ne))

#ifdef _OPENMP
static int64_t take_i64 (volatile int64_t* p);
static void release_i64 (volatile int64_t* p, int64_t val);
static struct packed_edge take_pe (volatile struct packed_edge* p);
static void release_pe (volatile struct packed_edge* p, struct packed_edge val);
#endif

#if defined(_OPENMP)
#define MAKE_RANDPERMUTE(name, elt_type, take, release) \
static void name (elt_type *A_in, int64_t nelem,mrg_state * __restrict  st) \
{ \
    elt_type * __restrict  A = A_in; \
    int64_t k; \
    \
    OMP("omp for") \
    for (k = 0; k < nelem; ++k) { \
        int64_t place; \
        elt_type Ak, Aplace; \
        \
        Ak = take (&A[k]); \
        \
        mrg_skip (st, 1, k, 0); \
        place = k + (int64_t)floor (mrg_get_double_orig (st) * (nelem - k)); \
        if (k != place) { \
        assert (place > k); \
        assert (place < nelem); \
        \
        Aplace = take (&A[place]); \
        \
        { \
            elt_type t; \
            t = A[place]; \
            A[place] = A[k]; \
            A[k] = t; \
        } \
    \
    { \
    elt_type t; \
    t = Aplace; \
    Aplace = Ak; \
    Ak = t; \
    } \
    \
    release (&A[place], Aplace); \
    } \
    release (&A[k], Ak); \
    } \
    }
#else
#define MAKE_RANDPERMUTE(name, elt_type, take, release) \
static void name (elt_type *A_in, int64_t nelem, mrg_state * __restrict  st) \
{ \
    elt_type * __restrict  A = A_in; \
    int64_t k; \
    \
    for (k = 0; k < nelem; ++k) { \
        int64_t place; \
        \
        place = k + (int64_t)floor (mrg_get_double_orig (st) * (nelem - k)); \
        \
        if (k != place) { \
            elt_type t; \
            t = A[place]; \
            A[place] = A[k]; \
            A[k] = t; \
        } \
    } \
 }
#endif

MAKE_RANDPERMUTE(randpermute_int64_t, int64_t, take_i64, release_i64)
MAKE_RANDPERMUTE(randpermute_packed_edge, struct packed_edge, take_pe, release_pe)

#undef MAKE_RANDPERMUTE

static void rmat_edge (struct packed_edge *out, int SCALE,double A, double B, double C, double D, const double *rn)
{
    size_t rni = 0;
    int64_t i = 0, j = 0;
    int64_t bit = ((int64_t)1) << (SCALE-1);

    while (1) {
        const double r = rn[rni++];
        if (r > A) {
            if (r <= A + B)
                j |= bit;
            else if (r <= A + B + C)
                i |= bit;
            else {
                j |= bit;
                i |= bit;
            }
        }
        if (1 == bit) break;

        A *= 0.95 + rn[rni++]/10;
        B *= 0.95 + rn[rni++]/10;
        C *= 0.95 + rn[rni++]/10;
        D *= 0.95 + rn[rni++]/10;
        {
            const double norm = 1.0 / (A + B + C + D);
            A *= norm; B *= norm; C *= norm;
        }
        D = 1.0 - (A + B + C);

        bit >>= 1;
    }
    write_edge(out, i, j);
}

static void permute_vertex_labels (struct packed_edge * __restrict  IJ, int64_t nedge, int64_t max_nvtx, mrg_state * __restrict  st, int64_t * __restrict  newlabel)
{
    int64_t k;

    OMP("omp for")
            for (k = 0; k < max_nvtx; ++k)
            newlabel[k] = k;

    randpermute_int64_t (newlabel, max_nvtx, st);

    OMP("omp for")
            for (k = 0; k < nedge; ++k)
            write_edge(&IJ[k],
                       newlabel[get_v0_from_edge(&IJ[k])],
            newlabel[get_v1_from_edge(&IJ[k])]);
}

static void permute_edgelist (struct packed_edge * __restrict  IJ, int64_t nedge, mrg_state *st)
{
    randpermute_packed_edge (IJ, nedge, st);
}

void rmat_edgelist (struct packed_edge *IJ_in, int64_t nedge, int SCALE,
                    double A, double B, double C)
{
    struct packed_edge * __restrict  IJ = IJ_in;
    int64_t* __restrict  iwork;
    double D = 1.0 - (A + B + C);

    iwork = xmalloc_large_ext ((size_t)((int64_t)(1L<<SCALE) * sizeof (*iwork)));

    OMP("omp parallel") {
        int64_t k;
        double * __restrict  Rlocal = malloc (NRAND(1) * sizeof (*Rlocal));
        mrg_state new_st = *(mrg_state*)prng_state;

        OMP("omp for")
                for (k = 0; k < nedge; ++k) {
            int k2;
            double * __restrict  Rlocal = malloc (NRAND(1) * sizeof (*Rlocal));
            mrg_skip (&new_st, 1, NRAND(1), 0);
            for (k2 = 0; k2 < NRAND(1); ++k2)
                Rlocal[k2] = mrg_get_double_orig (&new_st);
            rmat_edge (&IJ[k], SCALE, A, B, C, D, Rlocal);
        }

        OMP("omp single")
                mrg_skip (prng_state, 1, NRAND(nedge), 0);
        OMP("omp barrier");
        new_st = *(mrg_state*)prng_state;
        permute_vertex_labels (IJ, nedge, (int64_t)(1L<<SCALE), &new_st, iwork);
        OMP("omp single")
                mrg_skip (prng_state, 1, (uint_least64_t)(1L<<SCALE), 0);
        OMP("omp barrier");
        new_st = *(mrg_state*)prng_state;
        permute_edgelist (IJ, nedge, &new_st);
    }
    mrg_skip (prng_state, 1, nedge, 0);

    xfree_large (iwork);
}

#ifdef _OPENMP
#if defined(__GNUC__)||defined(__INTEL_COMPILER)
int64_t take_i64 (volatile int64_t *p)
{
    int64_t oldval;

    do {
        oldval = *p;
    } while (!(oldval >= 0 && __sync_bool_compare_and_swap (p, oldval, -1)));
    return oldval;
}

void release_i64 (volatile int64_t *p, int64_t val)
{
    assert (*p == -1);
    *p = val;
}

struct packed_edge take_pe (volatile struct packed_edge *p)
{
    struct packed_edge out;
    do {
        OMP("omp critical (TAKE)") {
            out = *p;
            if (get_v0_from_edge(&out) >= 0)
                write_edge((struct packed_edge*)p, -1, -1);
        }
    } while (get_v0_from_edge(&out) < 0);
    OMP("omp flush (p)");
    return out;
}

void release_pe (volatile struct packed_edge *p, struct packed_edge val)
{
    assert (get_v0_from_edge((const struct packed_edge*)p) == -1);
    OMP("omp critical (TAKE)") {
        *p = val;
        OMP("omp flush (p)");
    }
    return;
}
#else
int64_t take_i64 (volatile int64_t *p)
{
    int64_t out;
    do {
        OMP("omp critical (TAKE)") {
            out = *p;
            if (out >= 0)
                *p = -1;
        }
    } while (out < 0);
    OMP("omp flush (p)");
    return out;
}

void release_i64 (volatile int64_t *p, int64_t val)
{
    assert (*p == -1);
    OMP("omp critical (TAKE)") {
        *p = val;
        OMP("omp flush (p)");
    }
    return;
}

struct packed_edge take_pe (volatile struct packed_edge *p)
{
    struct packed_edge out;
    do {
        OMP("omp critical (TAKE)") {
            out = *p;
            if (get_v0_from_edge(&out) >= 0)
                write_edge((struct packed_edge*)p, -1, -1);
        }
    } while (get_v0_from_edge(&out) < 0);
    OMP("omp flush (p)");
    return out;
}

void release_pe (volatile struct packed_edge *p, struct packed_edge val)
{
    assert (get_v0_from_edge(p) == -1);
    OMP("omp critical (TAKE)") {
        *p = val;
        OMP("omp flush (p)");
    }
    return;
}
#endif
#endif
