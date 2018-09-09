#pragma once

#include <limits.h>     /* max */
#include <math.h>		/* ceil */
#include <inttypes.h>	/* int64_t */

#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/
#include "../opencl.h"
#include "kernel_kronecker.h"

/*	=============== Defines ===============	*/

#define OPTIMAL_GENERATOR_MOD 64
#define GENERATOR_ITEMS_BY_GROUP 64

#ifdef _WIN32
    #define GRAPH_OMP(x) __pragma(x)
#else
    #define GRAPH_OMP(x) _Pragma(#x)
#endif

#define KRONECKER_DEVICE_TYPE CL_DEVICE_TYPE_ALL

/*	=============== Functions ===============	*/

void generate_kronecker_egdes(int, size_t, mrg_state*, packed_edge*);
