#include "kronecker_generator.h"

void generate_kronecker_egdes(int scale, int64_t edge_number, mrg_state* seed, packed_edge* edges)
{
	/*	variable utiles	*/
	int i;
	int edge;
	int mul;
	int	ii_bit;

	double ab = A+B;

	double c_norm = C/(1-ab);
	double a_norm = A/ab;

	/*	initialisation	*/
	for(edge=0 ; edge<edge_number ; ++edge)
	{
		edges[edge].v1 = 1;
		edges[edge].v0 = 1;
	}

	/*	parcours	*/
	for(i=0 ; i<scale ; ++i)
	{
		if(VERBOSE)
			printf("Edge generation : %d/%d\n",i+1,scale);
		mul = 1<<i;
		for(edge=0 ; edge<edge_number ; edge++)
		{
			ii_bit = ( mrg_get_double_orig(seed)>ab );
			edges[edge].v1 += mul * ( mrg_get_double_orig(seed) > (c_norm*ii_bit + a_norm*(!ii_bit)) );
			edges[edge].v0 += mul * ii_bit;
		}
	}

	/* permutation aleatoire	*/
	random_permutation(1<<scale, edge_number, edges);
}

//TODO la permutation est naze !! a optimiser

#include <stdlib.h>
static void shuffle(int* array, int64_t l)
{
	int i;
	for(i=0 ; i<l ; i++)
	{
		int j = i+rand()/(RAND_MAX / (l-i)+1);
		int t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

void random_permutation(int numb_node,int64_t edge_number, packed_edge* edges)
{
	int i;
	int* vec = (int*)xmalloc(numb_node * sizeof(int));
	for(i=0 ; i<numb_node;i++)
		vec[i] = i;
	shuffle(vec,numb_node);

	for(i=0 ; i<edge_number ; i++)
	{
		edges[i].v0 = vec[edges[i].v0];
		edges[i].v1 = vec[edges[i].v1];
	}
	xfree_large(vec);
}
