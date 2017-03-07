#ifndef __KRONECKER_GENERATOR__
#define __KRONECKER_GENERATOR__

#include <inttypes.h>			/*	int64_t	*/
#include "../edge_struct.h"		/*	packed_edge	*/
#include "../splittable_mrg.h"	/*	mrg_state	*/
#include "../options.h"			/*	for A B C D	*/
#include "../xalloc.h"			/*	xmalloc	*/

void generate_kronecker_egdes(int, int64_t, mrg_state*, packed_edge*);
void random_permutation(int,int64_t,packed_edge*);

#endif

