#ifndef __COMPAT__
#define __COMPAT__

#ifdef _OPENMP
#define OMP(x) _Pragma(x)
#else
#define OMP(x)
#endif

#endif
