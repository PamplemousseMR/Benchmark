#ifndef COMPAT_HEADER_
#define COMPAT_HEADER_

#ifdef _OPENMP
	#define OMP(x) _Pragma(x)
	#include <omp.h>
#else
	#define OMP(x)
#endif

#endif /* COMPAT_HEADER_ */
