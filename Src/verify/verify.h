#ifndef __VERIFY__
#define __VERIFY__

#include "../compat.h"
#include <stdio.h>				/*	sprintf	*/
#include <stdint.h>				/*	int64_t	*/
#include <assert.h>				/*	assert	*/

#include "../edge_struct.h"		/*	packed_edge	*/
#include "../xalloc.h"			/*	xmalloc	*/

int64_t verify_bfs_tree (int64_t*, int64_t ,int64_t , const packed_edge*, int64_t);

#endif
