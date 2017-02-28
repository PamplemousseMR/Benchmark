#ifndef COMMUN_H
#define COMMUN_H
/*!
 * \file commun.h
 * \brief contient les define et autre chose commune a tout les autre fichiers
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */


/*!
 * \def thr
 * \brief for throw
 */
#ifdef __linux__
#define thr std::exception
#elif __MINGW32__
#define thr std::exception
#elif _WIN32
#define thr ...
#endif

/*!
 * \def A_PROB
 * \brief a probability for kronecker
 */
#define A_PROB 0.57
/*!
 * \def B_PROB
 * \brief b probability for kronecker
 */
#define B_PROB 0.19
/*!
 * \def C_PROB
 * \brief c probability for kronecker
 */
#define C_PROB 0.19

#endif // COMMUN_H
