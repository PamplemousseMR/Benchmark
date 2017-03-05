#ifndef PRNG_HEADER_
#define PRNG_HEADER_

#include "compat.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include "generator/splittable_mrg.h"

class Prng
{

	private:

		static uint64_t userseed;
		static uint_fast32_t prng_seed[5];
		static mrg_state prng_state_store;
		static void* prng_state;

		static void make_mrg_seed(uint64_t , uint_fast32_t[5]);

	public:

		static void init_random();
		static void* getPrngState();
		static uint64_t getUserseed();

};

#endif /* PRNG_HEADER_ */
