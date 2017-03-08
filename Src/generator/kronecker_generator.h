#ifndef __KRONECKER_GENERATOR__
#define __KRONECKER_GENERATOR__

#include <inttypes.h>			/*	int64_t	*/
#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/

/*	=============== Defines ===============	*/

#ifdef GRAPH_GENERATOR_OMP
	#include <omp.h>
	#define	GRAPH_OMP(X) _Pragma(X)
#else
	#define	GRAPH_OMP(X)
#endif

/*	=============== Functions ===============	*/

void generate_kronecker_egdes(int, int64_t, mrg_state*, packed_edge*);

#endif

