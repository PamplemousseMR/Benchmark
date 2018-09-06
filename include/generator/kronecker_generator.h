#ifndef __KRONECKER_GENERATOR__
#define __KRONECKER_GENERATOR__

#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/
#include "../opencl.h"
#include "kernel_kronecker.h"

#include <CL/cl.h>      /* OpenCL */
#include <limits.h>     /* max */
#include <math.h>		/* ceil */
#include <inttypes.h>	/* int64_t */
#include <omp.h>

#define OPTIMAL_GENERATOR_MOD 32
#define GENERATOR_ITEMS_BY_GROUP 32


/*	=============== Defines ===============	*/

/* les defines utiles */

#ifdef _WIN32
    #define GRAPH_OMP(x) __pragma(x)
#else
    #define GRAPH_OMP(x) _Pragma(#x)
#endif

#ifdef OPENCL_GPU
    #define KRONECKER_DEVICE_TYPE CL_DEVICE_TYPE_GPU
#elif OPENCL_CPU
    #define KRONECKER_DEVICE_TYPE CL_DEVICE_TYPE_CPU
#elif OPENCL_ACCELERATOR
    #define KRONECKER_DEVICE_TYPE CL_DEVICE_TYPE_ACCELERATOR
#else
	#define KRONECKER_DEVICE_TYPE CL_DEVICE_TYPE_ALL
#endif

/*	=============== Functions ===============	*/

void generate_kronecker_egdes(int, size_t, mrg_state*, packed_edge*);

#endif

