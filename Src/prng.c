#include "prng.h"

uint64_t userseed;
uint_fast32_t prng_seed[5];
static mrg_state prng_state_store;
void *prng_state = &prng_state_store;

static void make_mrg_seed(uint64_t userseed, uint_fast32_t* seed) {
  seed[0] = (userseed & 0x3FFFFFFF) + 1;
  seed[1] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
  seed[2] = (userseed & 0x3FFFFFFF) + 1;
  seed[3] = ((userseed >> 30) & 0x3FFFFFFF) + 1;
  seed[4] = ((userseed >> 60) << 4) + (userseed >> 60) + 1;
}

void init_random (void)
{
  long seed = -1;
  char* buf = NULL;
  size_t sz = 0;

  if (_dupenv_s(&buf,&sz,"SEED")) {
    errno = 0;
    seed = strtol((char*)_dupenv_s(&buf,&sz,"SEED"), NULL, 10);
    if (errno) seed = -1;
  }

  if (seed < 0) seed = 0xDECAFBAD;
  userseed = seed;
  make_mrg_seed (seed, prng_seed);
  mrg_seed(&prng_state_store, prng_seed);
}
