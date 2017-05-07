#ifndef GRAPH_VERIFY_OCL

#include "verify.h"

int64_t verify_bfs_tree (int64_t* bfs_tree, int64_t max_bfsvtx, int64_t root,const packed_edge* IJ, int64_t nedge)
{
	/* variables utils */
	int i, end;
	int64_t neither_in, both_in, respect_tree_level, x, y;
	int64_t *slice, *level, *P, *mask;

	int err = 0;
	int j=0;
	int k=0;
	int nedge_traversed=0;
	int nslice_value = 0;
	const int64_t nvalues = max_bfsvtx+1;

	/* verification basique */
	if(bfs_tree[root] != root || root > max_bfsvtx)
	{
		err = -999;
		goto done;
	}

	/* Calcul du nombre de sommets possédant un parent */
	VERIFY_OMP(omp parallel for shared(bfs_tree) reduction(+:nslice_value))
	for(i = 0; i<nvalues; ++i)
	{
		if(bfs_tree[i]>=0)
			++nslice_value;
	}

	/* Tableau des indices des sommets possédant un parent */
	slice = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);
	/* Tableau utile pour les calculs */
	level = (int64_t*)xmalloc(sizeof(int64_t)*nvalues);
	/* Tableau des parents des sommets */
	P = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);
	/* Tableau vérifiant si un sommet parent est le sommet racine */
	mask = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);

	for(i = 0; i<nvalues; ++i)
	{
		level[i] = bfs_tree[i]>=0;
		if(level[i]==1)
		{
			slice[j] = i;
			P[j] = bfs_tree[i];
			mask[j] = bfs_tree[i] != root;
			++j;
		}
	}

	do
	{
		end = 0;
		VERIFY_OMP(omp parallel for shared(mask, level, slice, P, bfs_tree, root, end))
		for(i = 0; i<nslice_value; ++i)
		{
			if(mask[i] == 1)
			{
				VERIFY_OMP(omp critical)
				++level[slice[i]];
				P[i] = bfs_tree[P[i]];
				mask[i] = P[i] != root;
				end = 1;
			}
		}
		++k;
		if(k>nvalues)
		{
			err = -3;
			goto done;
		}

	}while(end);

	VERIFY_OMP(omp parallel for shared(IJ, level, err) private(x, y, neither_in, both_in, respect_tree_level) reduction(+:nedge_traversed))
	for(i = 0 ; i<nedge ; ++i)
	{
		x = IJ[i].v0;
		y = IJ[i].v1;

		neither_in = level[x] == -1 && level[y] == -1;
		both_in = level[x] >= 0 && level[y] >= 0;
		if(!(neither_in || both_in))
		{
			err = -4;
			VERIFY_OMP(omp flush(err));
		}
		respect_tree_level = (level[x] - level[y]);
		respect_tree_level = (respect_tree_level >=0 ? respect_tree_level : -respect_tree_level) <= 1;
		if(!(neither_in || respect_tree_level))
		{
			err = -5;
			VERIFY_OMP(omp flush(err));
		}
		++nedge_traversed;
	}

	done:

	xfree_large(mask);
	xfree_large(P);
	xfree_large(level);
	xfree_large(slice);

	if(err) return err;
	return nedge_traversed;
}

#endif
