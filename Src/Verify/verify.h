#ifndef VERIFY_HEADER_
#define VERIFY_HEADER_

#include "../compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#include "../graph_struct.h"
#include "../xalloc.h"

class Verify
{

	public:

		static int64_t verify_bfs_tree(int64_t*, int64_t ,int64_t , const packed_edge*, int64_t);

};

#endif /* VERIFY_HEADER_ */
