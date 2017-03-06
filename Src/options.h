#ifndef __OPTIONS__
#define __OPTIONS__

#include "compat.h"
#include <stdio.h>		/*	printf	*/
#include <stdlib.h>		/*	EXIT_SUCCESS	*/
#include <errno.h>		/*	errno	*/
#include <inttypes.h>	/*	int64_t	*/
#include <string.h>		/*	strdup	*/
#ifdef _WIN32
#include <windows.h>
#include "getopt.h"		/*	getopt	*/
#else
#include <unistd.h>		/*	getopt	*/
#endif

/*	=============== Defines ===============	*/

/*!
 * \def NAME
 * \brief Le nom du programme.
 */
#define NAME "Benchmark"
/*!
 * \def VERSION
 * \brief La version du programme.
 */
#define VERSION 1.0
/*!
 * \def A_PARAM
 * \brief Probabilite de A pour la generation des aretes.
 */
#define A_PARAM 0.57
/*!
 * \def B_PARAM
 * \brief Probabilite de B pour la generation des aretes.
 */
#define B_PARAM 0.19
/*!
 * \def C_PARAM
 * \brief Probabilite de C pour la generation des aretes.
 */
#define C_PARAM 0.19
/*!
 * \def NBFS_max
 * \brief Nombre maximum de sommet choisit pour les parcours en largeurs.
 */
#define NBFS_MAX 64
/*!
 * \def default_SCALE
 * \brief Log2 du nombre de sommet par default.
 */
#define DEFAULT_SCALE ((int64_t)14)
/*!
 * \def default_edgefactor
 * \brief Ratio d'aretes par default.
 */
#define DEFAULT_EDGEFACTOR ((int64_t)16)

/*	=============== Global ===============	*/

extern int VERBOSE;
extern char *dumpname;
extern char *rootname;
extern double A, B, C, D;
extern int NBFS;
extern int64_t SCALE;
extern int64_t edgefactor;

/*	=============== Functions ===============	*/

/*!
 * \fn void get_options(int, char**)
 * \brief permet de charger les arguments du programme dans les variables globales
 * \param int : le nombre d'argument
 * \param char** : le tableau de caracteres
 */
void get_options(int, char**);

#endif
