#ifndef __COMPAT__
#define __COMPAT__

/*!
 * \file compat.h
 * \param Fichier qui permet d'inclure le bon systeme de calcule.
 * \author Appert Kevin
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

#ifdef _OPENMP
    #ifdef _WIN32
    #define OMP(x) __pragma(x)
    #else
    #define OMP(x) _Pragma(x)
    #endif

    #include <omp.h>

#else
#define OMP(x)
#endif

#endif
