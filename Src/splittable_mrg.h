#ifndef SPLITTABLE_MRG_H
#define SPLITTABLE_MRG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mrg_state {
  uint_fast32_t z1, z2, z3, z4, z5;
} mrg_state;

typedef struct mrg_transition_matrix {
    uint_fast32_t s, t, u, v, w;
    uint_fast32_t a, b, c, d;
} mrg_transition_matrix;

extern uint64_t userseed;
extern uint_fast32_t prng_seed[5];
extern void* prng_state;

uint_fast32_t mrg_get_uint_orig(mrg_state* state);
double mrg_get_double_orig(mrg_state* state);
void mrg_seed(mrg_state* st, const uint_fast32_t seed[5]);
void mrg_skip(mrg_state* state, uint_least64_t exponent_high, uint_least64_t exponent_middle,uint_least64_t exponent_low);
void make_mrg_seed(uint64_t userseed1, uint64_t userseed2, uint_fast32_t* seed);
void init_random(void);

#ifdef __cplusplus
}
#endif

#endif
