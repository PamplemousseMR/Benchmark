#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct packed_edge {
  int64_t v0;
  int64_t v1;
} packed_edge;

static __inline int64_t get_v0_from_edge(const packed_edge* p) {
  return p->v0;
}

static __inline int64_t get_v1_from_edge(const packed_edge* p) {
  return p->v1;
}

static __inline void write_edge(packed_edge* p, int64_t v0, int64_t v1) {
  p->v0 = v0;
  p->v1 = v1;
}

#ifdef __cplusplus
}
#endif

#endif
