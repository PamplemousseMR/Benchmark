#include "make_graph.h"

#ifndef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr_in, packed_edge** result_ptr_in)
{

}
#endif /* !GRAPH_GENERATOR_MPI */

#ifdef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr, packed_edge** result_ptr)
{

}
#endif
