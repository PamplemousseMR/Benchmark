#ifndef __XALLOC__
#define __XALLOC__

#include "compat.h"
#include <stdlib.h>		/*	malloc	*/
#include <stdio.h>		/*	print	*/
#ifdef _WIN32
#include <windows.h>
#endif

/*	=============== Functions ===============	*/

void* xmalloc(size_t);
void* xmalloc_large(size_t);
void* xmalloc_large_ext(size_t);
void xfree_large(void*);

#endif
