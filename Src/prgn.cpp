#include "prng.h"

uint64_t Prng::userseed = 0;
uint_fast32_t Prng::prng_seed[5] = {0,0,0,0,0};
mrg_state Prng::prng_state_store;
void* Prng::prng_state = &prng_state_store;

void make_mrg_seed(uint64_t userseed, uint_fast32_t* seed)
{
  seed[0] = (userseed & 0x3FFFFFFF) + 1;
  seed[1] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
  seed[2] = (userseed & 0x3FFFFFFF) + 1;
  seed[3] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
  seed[4] = ((userseed >> 60) << 4) + (userseed >> 60) + 1;
}

void make_mrg_seed(uint64_t userseed1, uint64_t userseed2, uint_fast32_t* seed)
{
  seed[0] = (userseed1 & 0x3FFFFFFF) + 1;
  seed[1] = ((userseed1 >> 30) & 0x3FFFFFFF) + 1;
  seed[2] = (userseed2 & 0x3FFFFFFF) + 1;
  seed[3] = ((userseed2 >> 30) & 0x3FFFFFFF) + 1;
  seed[4] = ((userseed2 >> 60) << 4) + (userseed1 >> 60) + 1;
}

void Prng::init_random ()
{
	long seed = -1;
	#ifdef _WIN32
	char* buf = NULL;
	size_t sz = 0;
	if (_dupenv_s(&buf,&sz,"SEED"))
	#else
	if (getenv("SEED"))
	#endif
	{
		errno = 0;
		#ifdef _WIN32
		seed = strtol((char*)_dupenv_s(&buf,&sz,"SEED"), NULL, 10);
		#else
		seed = strtol(getenv("SEED"), NULL, 10);
		#endif
		if (errno)
			seed = -1;
	}

	if (seed < 0)
		seed = 0xDECAFBAD;
	userseed = seed;
	make_mrg_seed (seed, prng_seed);
	mrg_seed(&prng_state_store, prng_seed);
}

void* Prng::getPrngState()
{
	return Prng::prng_state;
}

uint64_t Prng::getUserseed()
{
	return Prng::userseed;
}
