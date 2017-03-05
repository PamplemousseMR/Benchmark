#ifndef MOD_ARITH_H
#define MOD_ARITH_H

#ifdef FAST_64BIT_ARITHMETIC
#include "mod_arith_64bit.h"
#else
#include "mod_arith_32bit.h"
#endif

#endif
