#ifndef __KRONECKER_GENERATOR__
#define __KRONECKER_GENERATOR__

#include <inttypes.h>			/*	int64_t	*/
#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/

/*	=============== Defines ===============	*/

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

/*	=============== Functions ===============	*/

void generate_kronecker_egdes(int, int64_t, mrg_state*, packed_edge*);

#endif

