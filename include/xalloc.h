#pragma once

/*	=============== Includes ===============	*/

#include "compat.h"
#include <stdlib.h>		/*	malloc	*/
#include <stdio.h>		/*	print	*/
#ifdef _WIN32
#include "mman_win.h"   /*  mmap    */
#else
#include <sys/mman.h>   /*  mmap    */
#endif

/*	=============== Functions ===============	*/

void* xmalloc(size_t);
void* xmalloc_large(size_t);
void* xmalloc_large_ext(size_t);
void xfree_large(void*);
