#ifndef COMPAT_HEADER_
#define COMPAT_HEADER_

#ifdef _OPENMP
#define OMP(x) _Pragma(x)
#else
#define OMP(x)
#endif

#endif
