#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include <stdint.h>

#ifdef GENERATOR_USE_PACKED_EDGE_TYPE

typedef struct packed_edge {
  uint32_t v0_low;
  uint32_t v1_low;
  uint32_t high; /* v1 in high half, v0 in low half */
} packed_edge;

#else

typedef struct packed_edge {
  int64_t v0;
  int64_t v1;
} packed_edge;

#endif

static int64_t get_v0_from_edge(const packed_edge* p) {
  return p->v0;
}

static int64_t get_v1_from_edge(const packed_edge* p) {
  return p->v1;
}

static void write_edge(packed_edge* p, int64_t v0, int64_t v1) {
  p->v0 = v0;
  p->v1 = v1;
}

#endif /* GRAPH_GENERATOR_H */
