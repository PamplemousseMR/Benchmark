#include "verify.h"

static int compute_levels (int64_t * level,int64_t nv, const int64_t * __restrict  bfs_tree, int64_t root)
{
  int err = 0;

  OMP(omp parallel shared(err)) {
    int terr;
    int64_t k;

	OMP(omp for)
      for (k = 0; k < nv; ++k)
	level[k] = (k == root? 0 : -1);

	OMP(omp for)
      for (k = 0; k < nv; ++k) {
	if (level[k] >= 0) continue;
	terr = err;
	if (!terr && bfs_tree[k] >= 0 && k != root) {
	  int64_t parent = k;
	  int64_t nhop = 0;
	  while (parent >= 0 && level[parent] < 0 && nhop < nv) {
	    assert (parent != bfs_tree[parent]);
	    parent = bfs_tree[parent];
	    ++nhop;
	  }
      if (nhop >= nv) terr = -1;
      if (parent < 0) terr = -2;

	  if (!terr) {
	    nhop += level[parent];
	    parent = k;
	    while (level[parent] < 0) {
	      assert (nhop > 0);
	      level[parent] = nhop--;
	      parent = bfs_tree[parent];
	    }
	    assert (nhop == level[parent]);

#if !defined(NDEBUG)
	    nhop = 0;
	    parent = k;
	    int64_t lastlvl = level[k]+1;
	    while (level[parent] > 0) {
	      assert (lastlvl == 1 + level[parent]);
	      lastlvl = level[parent];
	      parent = bfs_tree[parent];
	      ++nhop;
	    }
#endif
	  }
	}
	if (terr) { err = terr;	OMP(omp flush (err)); }
      }
  }
  return err;
}

int64_t verify_bfs_tree (int64_t *bfs_tree_in, int64_t max_bfsvtx, int64_t root,const packed_edge* IJ_in, int64_t nedge)
{
  int64_t * __restrict  bfs_tree = bfs_tree_in;
  const int64_t* __restrict  IJ = (int64_t*)IJ_in;

  int err, nedge_traversed;
  int64_t * __restrict  seen_edge, * __restrict  level;

  const int64_t nv = max_bfsvtx+1;

  if (root > max_bfsvtx || bfs_tree[root] != root)
    return -999;

  err = 0;
  nedge_traversed = 0;
  seen_edge = xmalloc_large (2 * (nv) * sizeof (*seen_edge));
  level = &seen_edge[nv];

  err = compute_levels (level, nv, bfs_tree, root);

  if (err) goto done;

  OMP(omp parallel shared(err)) {
    int64_t k;
    int terr = 0;
	OMP(omp for)
      for (k = 0; k < nv; ++k)
	seen_edge[k] = 0;

	OMP(omp for reduction(+:nedge_traversed))
      for (k = 0; k < 2*nedge; k+=2) {
	const int64_t i = IJ[k];
	const int64_t j = IJ[k+1];
	int64_t lvldiff;
	terr = err;

	if (i < 0 || j < 0) continue;
	if (i > max_bfsvtx && j <= max_bfsvtx) terr = -10;
	if (j > max_bfsvtx && i <= max_bfsvtx) terr = -11;
	if (terr) { err = terr; OMP(omp flush(err)); }
    if (terr || i > max_bfsvtx)
	  continue;

	if (bfs_tree[i] >= 0 && bfs_tree[j] < 0) terr = -12;
	if (bfs_tree[j] >= 0 && bfs_tree[i] < 0) terr = -13;
	if (terr) { err = terr; OMP(omp flush(err)); }
    if (terr || bfs_tree[i] < 0)
	  continue;

	++nedge_traversed;
	if (i != j) {
	  if (bfs_tree[i] == j)
	    seen_edge[i] = 1;
	  if (bfs_tree[j] == i)
	    seen_edge[j] = 1;
	}
	lvldiff = level[i] - level[j];
	if (lvldiff > 1 || lvldiff < -1)
	  terr = -14;
	if (terr) { err = terr; OMP(omp flush(err)); }
      }

    if (!terr) {
	  OMP(omp for)
	for (k = 0; k < nv; ++k) {
	  terr = err;
	  if (!terr && k != root) {
	    if (bfs_tree[k] >= 0 && !seen_edge[k])
	      terr = -15;
	    if (bfs_tree[k] == k)
	      terr = -16;
		if (terr) { err = terr; OMP(omp flush(err)); }
	  }
	}
    }
  }
 done:

  xfree_large (seen_edge);
  if (err) return err;
  return nedge_traversed;
}
