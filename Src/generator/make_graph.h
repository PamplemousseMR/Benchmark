#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#include <stdint.h>
#include "../edge_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

void make_graph(int, int64_t, uint64_t, uint64_t, int64_t*, packed_edge**);

#ifdef __cplusplus
}
#endif

#endif
