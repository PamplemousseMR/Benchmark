#ifndef __KRONECKER_GENERATOR__
#define __KRONECKER_GENERATOR__

#include <inttypes.h>			/*	int64_t	*/
#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/

#ifdef GRAPH_GENERATOR_OCL
#include <CL/cl.h>				/*	openCL */
#include "../opencl.h"

#define OPTIMAL_MOD 32
#define ITEMS_BY_GROUP 32
#endif

/*	=============== Includes ===============	*/

#ifdef GRAPH_GENERATOR_OCL
    #include <CL/cl.h>      /* OpenCL */
    #include <limits.h>     /* max */
#endif

/*	=============== Defines ===============	*/

/*tests pour les defines*/
#ifdef GRAPH_GENERATOR_OCL
    #ifdef GRAPH_GENERATOR_OMPI
    #error OpenCL cannot run with OpenMPI
    #endif
#endif

/* les defines utiles */

#ifdef GRAPH_GENERATOR_OMP
    #ifdef _WIN32
		#define GRAPH_OMP(x) __pragma(x)
	#else
		#define GRAPH_OMP(x) _Pragma(#x)
    #endif

    #include <omp.h>

#else
	#define	GRAPH_OMP(X)
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

void generate_kronecker_egdes(int, int64_t, mrg_state*, packed_edge*);

#endif

