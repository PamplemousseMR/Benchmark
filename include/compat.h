#pragma once

#ifdef _OPENMP
    #ifdef _WIN32
        #define OMP(x) __pragma(x)
    #else
        #define OMP(x) _Pragma(#x)
    #endif
#else
    #define OMP(x)
#endif
