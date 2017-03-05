#include "make_graph.h"
#include "../xalloc.h"
#include <random>
#include <iostream>
#ifndef GRAPH_GENERATOR_MPI
void Graph::make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr_in, packed_edge** result_ptr_in)
{
	std::cout << "makegraph , log : " << log_numverts << " edges : " << M << std::endl;
	/* Add restrict to input pointers. */
	*nedges_ptr_in = M;
	*result_ptr_in = (packed_edge*)Xalloc::xmalloc(M * sizeof(packed_edge));

	int max = 1<<log_numverts;

	std::cout << "max sommet : " << max << std::endl;

	for(int i=0 ; i<M ; i++)
	{
		write_edge(&(*result_ptr_in)[i],rand()%(max+1),rand()%(max+1));
	}
}
#endif /* !GRAPH_GENERATOR_MPI */

#ifdef GRAPH_GENERATOR_MPI
void Graph::make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr, packed_edge** result_ptr)
{

}
#endif
