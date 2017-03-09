#include "kronecker_generator.h"

#define SHUFFLE(name,type)                                                  \
static void name(type* array, int64_t l, mrg_state* seed)                   \
{                                                                           \
    int i;                                                                  \
    int j;                                                                  \
    type t;                                                                 \
    for(i=0 ; i<l ; ++i)                                                    \
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
    for(i=0 ; i<numb_node;++i)
		vec[i] = i;

    suffle_int(vec,numb_node,seed);

    GRAPH_OMP(omp parallel for shared(edges,vec))
    for(i=0 ; i<edge_number ; ++i)
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

    int maxThread = omp_get_max_threads();
    uint_fast32_t* seeds = (uint_fast32_t*)xmalloc(maxThread * sizeof(uint_fast32_t));

	/*	initialisation	*/
    GRAPH_OMP(omp parallel for shared(edges))
    for(edge=0 ; edge<edge_number ; ++edge)
    {
        edges[edge].v1 = 1;
        edges[edge].v0 = 1;
    }

    GRAPH_OMP(omp parallel for shared(seeds))
    for(i=0 ; i<maxThread ; ++i)
       seeds[i] = mrg_get_uint_orig(seed);

	/*	parcours	*/
	for(i=0 ; i<scale ; ++i)
	{
		if(VERBOSE)
			printf("Edge generation : %d/%d\n",i+1,scale);

		mul = 1<<i;

        //GRAPH_OMP(omp parallel for shared(edges, mul, ab, c_norm, a_norm, seed) private(ii_bit))
        for(edge=0 ; edge<edge_number ; ++edge)
        {
            //printf("%d : %d -> %lf\n",omp_get_thread_num(),seeds[omp_get_thread_num()],(double)(rand(&seeds[omp_get_thread_num()]))/RAND_MAX);
            ii_bit = ( (double)(rand())/MRG_RAND_MAX > ab );
            edges[edge].v1 +=  mul * ( (double)(rand())/MRG_RAND_MAX > (c_norm*ii_bit + a_norm*(!ii_bit)) );
            edges[edge].v0 +=  mul * ii_bit;
        }
    }

    free(seeds);


    int64_t max = 0;
    int64_t min = MRG_RAND_MAX;

    for(edge=0 ; edge<edge_number ; ++edge)
    {
        if(edges[edge].v0 < min)
            min = edges[edge].v0;
        if(edges[edge].v1 < min)
            min = edges[edge].v1;

        if(edges[edge].v0 > max)
            max = edges[edge].v0;
        if(edges[edge].v1 > max)
            max = edges[edge].v1;

        //printf("(%d->%d)\n",edges[edge].v0,edges[edge].v1);
    }
    printf("(%d->%d)\n",min,max);

	/* permutation aleatoire des sommets	*/
    random_node_permutation(1<<scale, edge_number, edges,seed);
    /*	permutation ameatoire des aretes	*/
    random_edges_permutation(edge_number, edges, seed);
}
