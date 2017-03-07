#include "edge_generator.h"

#ifndef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t desired_edge, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr_in, packed_edge** result_ptr_in)
{
	*nedges_ptr_in = desired_edge;

	packed_edge* edges = (packed_edge*)xmalloc(desired_edge * sizeof(packed_edge));
	*result_ptr_in = edges;

    uint_fast32_t seed[5];
    make_mrg_seed(userseed1, userseed2, seed);

	generate_kronecker_egdes(log_numverts,desired_edge,(mrg_state*)seed,edges);
}
#endif

#ifdef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr, packed_edge** result_ptr) {

}
#endif
