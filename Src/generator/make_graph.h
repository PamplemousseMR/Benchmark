#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#ifdef GRAPH_GENERATOR_MPI
#include <mpi.h>
#endif
#ifdef GRAPH_GENERATOR_OMP
#include <omp.h>
#endif
#include "graph_generator.h"

#ifdef __cplusplus
extern "C" {
#endif

void make_graph(
  /* in */ int          /* log_2 of vertex count */,
  /* in */ int64_t    /* Target number of edges */,
  /* in */ uint64_t        /* Arbitrary 64-bit seed value */,
  /* in */ uint64_t        /* Arbitrary 64-bit seed value */,
  /* out */ int64_t*           /* Number of generated edges */,
  /* out */ packed_edge**     /* Array of edges; allocated by*/
);

#ifdef __cplusplus
}
#endif

#endif /* MAKE_GRAPH_H */
