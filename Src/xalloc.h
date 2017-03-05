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

void* xmalloc(size_t);
void* xmalloc_large (size_t);
void* xmalloc_large_ext (size_t);
void xfree_large(void*);

#endif
