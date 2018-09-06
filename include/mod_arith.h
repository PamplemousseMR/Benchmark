#pragma once

#ifdef FAST_64BIT_ARITHMETIC
#include "mod_arith_64bit.h"
#else
#include "mod_arith_32bit.h"
#endif
