#ifndef PRNG_HEADER_
#define PRNG_HEADER_

#include "compat.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include "splittable_mrg.h"

void init_random(void);

extern uint64_t userseed;
extern uint_fast32_t prng_seed[5];
extern void *prng_state;

#endif /* PRNG_HEADER_ */
