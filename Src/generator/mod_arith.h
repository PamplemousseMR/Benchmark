/* Copyright (C) 2010 The Trustees of Indiana University.                  */
/*                                                                         */
/* Use, modification and distribution is subject to the Boost Software     */
/* License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at */
/* http://www.boost.org/LICENSE_1_0.txt)                                   */
/*                                                                         */
/*  Authors: Jeremiah Willcock                                             */
/*           Andrew Lumsdaine                                              */

#ifndef MOD_ARITH_H
#define MOD_ARITH_H

#include "user_settings.h"


#ifdef FAST_64BIT_ARITHMETIC
#include "mod_arith_64bit.h"
#else
#include "mod_arith_32bit.h"
#endif
#

#endif /* MOD_ARITH_H */
