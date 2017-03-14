#ifndef __OPTIONS__
#define __OPTIONS__

/*!
 * \file options.h
 * \param Fichier qui permet de traiter et stocker les arguments du programme.
 * \author Appert Kevin
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

/*	=============== Includes ===============	*/

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
 * \def DEFAULT_SCALE
 * \brief Log2 du nombre de sommet par default.
 */
#define DEFAULT_SCALE ((int64_t)22)
/*!
 * \def DEFAULT_EDGEFACTOR
 * \brief Ratio d'aretes par default.
 */
#define DEFAULT_EDGEFACTOR ((int64_t)16)

/*	=============== Global ===============	*/

/*!
 * \brief Pour permettre des affichages supplementaires.
 */
extern int VERBOSE;
/*!
 * \brief Le chemin du fichier contenent les aretes.
 */
extern char *dumpname;
/*!
 * \brief Le chemin du fichier contenent les racines.
 */
extern char *rootname;
/*!
 * \brief La probabilite de A
 */
extern double A;
/*!
 * \brief La probabilite de B
 */
extern double B;
/*!
 * \brief La probabilite de C
 */
extern double C;
/*!
 * \brief La probabilite de D
 */
extern double D;
/*!
 * \brief Le nombre maximum de parcourt en largeur a calculer.
 */
extern int NBFS;
/*!
 * \brief Le log2 du nombre de sommets.
 */
extern int64_t SCALE;
/*!
 * \brief Le ratio du nombre d'aretes.
 */
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
