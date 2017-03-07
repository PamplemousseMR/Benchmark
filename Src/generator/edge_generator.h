#ifndef __EDGE_GENERATOR__
#define __EDGE_GENERATOR__

#include <stdint.h>					/*	int64_t	*/
#include "../edge_struct.h"			/*	packed_edge	*/
#include "kronecker_generator.h"	/*	generate_kronecker_egdes	*/
#include "../splittable_mrg.h"		/*	mrg_state	*/
#include "../xalloc.h"				/*	xmalloc	*/

void make_graph(int, int64_t, uint64_t, uint64_t, int64_t*, packed_edge**);

#endif
