#include "splittable_mrg.h"

#include "mrg_transitions.c"

static uint_fast32_t prng_seed[5];
static mrg_state prng_state_store;

uint64_t userseed;
void* prng_state = &prng_state_store;

static void make_seed(uint64_t userseed, uint_fast32_t* seed)
{
	seed[0] = (userseed & 0x3FFFFFFF) + 1;
	seed[1] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
	seed[2] = (userseed & 0x3FFFFFFF) + 1;
	seed[3] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
	seed[4] = ((userseed >> 60) << 4) + (userseed >> 60) + 1;
}

static void mrg_apply_transition(const mrg_transition_matrix* __restrict  mat, const mrg_state* __restrict  st, mrg_state* r)
{
	uint_fast32_t o1 = mod_mac_y(mod_mul(mat->d, st->z1), mod_mac4(0, mat->s, st->z2, mat->a, st->z3, mat->b, st->z4, mat->c, st->z5));
	uint_fast32_t o2 = mod_mac_y(mod_mac2(0, mat->c, st->z1, mat->w, st->z2), mod_mac3(0, mat->s, st->z3, mat->a, st->z4, mat->b, st->z5));
	uint_fast32_t o3 = mod_mac_y(mod_mac3(0, mat->b, st->z1, mat->v, st->z2, mat->w, st->z3), mod_mac2(0, mat->s, st->z4, mat->a, st->z5));
	uint_fast32_t o4 = mod_mac_y(mod_mac4(0, mat->a, st->z1, mat->u, st->z2, mat->v, st->z3, mat->w, st->z4), mod_mul(mat->s, st->z5));
	uint_fast32_t o5 = mod_mac2(mod_mac3(0, mat->s, st->z1, mat->t, st->z2, mat->u, st->z3), mat->v, st->z4, mat->w, st->z5);
	r->z1 = o1;
	r->z2 = o2;
	r->z3 = o3;
	r->z4 = o4;
	r->z5 = o5;
}

static void mrg_step(const mrg_transition_matrix* mat, mrg_state* state)
{
	mrg_apply_transition(mat, state, state);
}

static void mrg_orig_step(mrg_state* state)
{
	uint_fast32_t new_elt = mod_mac_y(mod_mul_x(state->z1), state->z5);
	state->z5 = state->z4;
	state->z4 = state->z3;
	state->z3 = state->z2;
	state->z2 = state->z1;
	state->z1 = new_elt;
}

static void mrg_seed(mrg_state* st, const uint_fast32_t seed[5])
{
    st->z1 = seed[0];
    st->z2 = seed[1];
    st->z3 = seed[2];
    st->z4 = seed[3];
    st->z5 = seed[4];
}

uint_fast32_t mrg_get_uint_orig(mrg_state* state)
{
	mrg_orig_step(state);
	return state->z1;
}

double mrg_get_double_orig(mrg_state* state)
{
    return (double)mrg_get_uint_orig(state) * .000000000465661287524579692 + (double)mrg_get_uint_orig(state) * .0000000000000000002168404346990492787;
}

void make_mrg_seed(uint64_t userseed1, uint64_t userseed2, uint_fast32_t* seed)
{
	seed[0] = (userseed1 & 0x3FFFFFFF) + 1;
	seed[1] = ((userseed1 >> 30) & 0x3FFFFFFF) + 1;
	seed[2] = (userseed2 & 0x3FFFFFFF) + 1;
	seed[3] = ((userseed2 >> 30) & 0x3FFFFFFF) + 1;
	seed[4] = ((userseed2 >> 60) << 4) + (userseed1 >> 60) + 1;
}

void init_random (void)
{
    long seed = -1;

    #ifdef _WIN32
    char* buf = NULL;
    size_t sz = 0;
    if (_dupenv_s(&buf,&sz,"SEED"))
    #else
    if (getenv("SEED"))
    #endif
    {
        errno = 0;
        #ifdef _WIN32
        seed = strtol((char*)_dupenv_s(&buf,&sz,"SEED"), NULL, 10);
        #else
        seed = strtol((char*)getenv("SEED"), NULL, 10);
        #endif
        if (errno)
            seed = -1;
    }

    if (seed < 0)
        seed = 0xDECAFBAD;
    userseed = seed;
    make_seed (seed, prng_seed);
    mrg_seed(&prng_state_store, prng_seed);
}
