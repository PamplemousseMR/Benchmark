#ifndef __COMPAT__
#define __COMPAT__

/*!
 * \file compat.h
 * \param Fichier qui permet d'inclure le bon systeme de calcule.
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

#ifdef _OPENMP
#define OMP(x) _Pragma(x)
#else
#define OMP(x)
#endif

#endif
