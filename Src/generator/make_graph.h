#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#include <stdint.h>
#include "../graph_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Simplified interface for users; implemented in different ways on different
 * platforms. */
void make_graph(
  /* in */ int log_numverts          /* log_2 of vertex count */,
  /* in */ int64_t desired_nedges    /* Target number of edges */,
  /* in */ uint64_t userseed1        /* Arbitrary 64-bit seed value */,
  /* in */ uint64_t userseed2        /* Arbitrary 64-bit seed value */,
  /* out */ int64_t* nedges          /* Number of generated edges */,
  /* out */ packed_edge** result     /* Array of edges; allocated by
                                        make_graph() but must be freed using
                                        free() by user */
  /* See functions in graph_generator.h for the definition of and how to
   * manipulate packed_edge objects (functions are write_edge,
   * get_v0_from_edge, get_v1_from_edge). */
);

#ifdef __cplusplus
}
#endif

#endif /* MAKE_GRAPH_H */
