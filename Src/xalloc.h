#ifndef XALLOC_HEADER_
#define XALLOC_HEADER_

#include "compat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
	#include <sys/mman.h>
#endif

#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0
#endif

#ifndef MAP_POPULATE
#define MAP_POPULATE 0
#endif

#ifndef MAP_NOSYNC
#define MAP_NOSYNC 0
#endif

class Xalloc
{

	public:

		static void* xmalloc(size_t);
		static void* xmalloc_large(size_t);
		static void xfree_large(void *);
		static void* xmalloc_large_ext(size_t);

};

#endif /* XALLOC_HEADER_ */
