#ifndef GRAPH_GENERATOR_OMPI
#ifndef GRAPH_GENERATOR_OCL

#include "kronecker_generator.h"

#define SHUFFLE(name,type)                                                  \
static void name(type* array, int64_t l, mrg_state* seed)                   \
{                                                                           \
	int i;                                                                  \
	int j;                                                                  \
	type t;                                                                 \
	for(i=0 ; i<l ; i++)                                                    \
	{                                                                       \
		j = (int)(i+mrg_get_uint_orig(seed)/(MRG_RAND_MAX / (l-i)+1));		\
			t = array[j];                                                   \
			array[j] = array[i];                                            \
			array[i] = t;                                                   \
	}                                                                       \
}

SHUFFLE(suffle_int,int)
SHUFFLE(suffle_edges,packed_edge)

#undef SHUFFLE

static void random_node_permutation(int numb_node,int64_t edge_number, packed_edge* edges, mrg_state* seed)
{
	int i;
	int* vec = (int*)xmalloc(numb_node * sizeof(int));

    GRAPH_OMP(omp parallel for shared(vec))
	for(i=0 ; i<numb_node;i++)
		vec[i] = i;

	suffle_int(vec,numb_node,seed);

    GRAPH_OMP(omp parallel for shared(edges,vec))
	for(i=0 ; i<edge_number ; i++)
	{
		edges[i].v0 = vec[edges[i].v0-1];
		edges[i].v1 = vec[edges[i].v1-1];
	}

	xfree_large(vec);
}

static void random_edges_permutation(int64_t edge_number, packed_edge* edges, mrg_state* seed)
{
	suffle_edges(edges,edge_number, seed);
}

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

	int maxThreads = omp_get_max_threads();
	mrg_state* seeds = (mrg_state*)xmalloc(sizeof(mrg_state) * maxThreads);

	/*	initialisation	*/
    GRAPH_OMP(omp parallel for shared(edges))
	for(edge=0 ; edge<edge_number ; ++edge)
	{
		edges[edge].v1 = 1;
		edges[edge].v0 = 1;
	}

	GRAPH_OMP(omp parallel for shared(seeds))
	for(i=0 ; i<maxThreads ; ++i)
		make_mrg_seed(mrg_get_uint_orig(seed), mrg_get_uint_orig(seed), (uint_fast32_t*)&seeds[i]);

	/*	parcours	*/
	for(i=0 ; i<scale ; ++i)
	{
		if(VERBOSE)
			printf("Edges generation : %d/%d\n",i+1,scale);

		mul = 1<<i;

        GRAPH_OMP(omp parallel for shared(edges, mul, ab, c_norm, a_norm, seeds) private(ii_bit))
		for(edge=0 ; edge<edge_number ; edge++)
		{
			ii_bit = ( mrg_get_double_orig(&seeds[omp_get_thread_num()])>ab );
			edges[edge].v1 +=  mul * ( mrg_get_double_orig(&seeds[omp_get_thread_num()]) > (c_norm*ii_bit + a_norm*(!ii_bit)) );
			edges[edge].v0 +=  mul * ii_bit;
		}
	}

	free(seeds);

	/* permutation aleatoire des sommets	*/
	random_node_permutation(1<<scale, edge_number, edges,seed);
	/*	permutation ameatoire des aretes	*/
	random_edges_permutation(edge_number, edges, seed);
}

#endif /* GRAPH_GENERATOR_OCL */
#endif /* GRAPH_GENERATOR_OMPI */
