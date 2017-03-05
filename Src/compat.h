#ifndef COMPAT_HEADER_
#define COMPAT_HEADER_

#ifdef _OPENMP
#define OMP(x) _Pragma(x)
#include <omp.h>
#else
#define OMP(x)
//static int omp_get_thread_num (void) { return 0; }
//static int omp_get_num_threads (void) { return 1; }
#endif

#endif
