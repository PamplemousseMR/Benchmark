#ifndef __MOD_ARITH__
#define __MOD_ARITH__

#ifdef FAST_64BIT_ARITHMETIC
#include "mod_arith_64bit.h"
#else
#include "mod_arith_32bit.h"
#endif

#endif
