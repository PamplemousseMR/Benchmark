#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "user_settings.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GENERATOR_USE_PACKED_EDGE_TYPE

typedef struct packed_edge {
  uint32_t v0_low;
  uint32_t v1_low;
  uint32_t high; /* v1 in high half, v0 in low half */
} packed_edge;

static __inline int64_t get_v0_from_edge(const packed_edge* p) {
  return (p->v0_low | ((int64_t)((int16_t)(p->high & 0xFFFF)) << 32));
}

static __inline int64_t get_v1_from_edge(const packed_edge* p) {
  return (p->v1_low | ((int64_t)((int16_t)(p->high >> 16)) << 32));
}

static __inline void write_edge(packed_edge* p, int64_t v0, int64_t v1) {
  p->v0_low = (uint32_t)v0;
  p->v1_low = (uint32_t)v1;
  p->high = ((v0 >> 32) & 0xFFFF) | (((v1 >> 32) & 0xFFFF) << 16);
}

#else

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

#endif

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_GENERATOR_H */
