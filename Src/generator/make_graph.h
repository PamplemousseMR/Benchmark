#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#ifdef GRAPH_GENERATOR_MPI
#include <mpi.h>
#endif
#ifdef GRAPH_GENERATOR_OMP
#include <omp.h>
#endif

#include "../graph_struct.h"

class Graph
{

	public:

		static void make_graph(int, int64_t, uint64_t, uint64_t, int64_t*, packed_edge**);

};


#endif /* MAKE_GRAPH_H */
