#ifndef __EDGE_GENERATOR__
#define __EDGE_GENERATOR__
/*!
 * \file edge_generator.h
 * \param Fichier contenent la fonction qui permet de generer une liste d'arete avec la methode de kronecker.
 * \author Appert Kevin
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

#include <stdint.h>					/*	int64_t	*/
#include "../edge_struct.h"			/*	packed_edge	*/
#include "kronecker_generator.h"	/*	generate_kronecker_egdes	*/
#include "../splittable_mrg.h"		/*	mrg_state	*/
#include "../xalloc.h"				/*	xmalloc	*/

/*!
 * \fn void make_graph(int, int64_t, uint64_t, uint64_t, int64_t*, packed_edge**)
 * \brief Remplie la liste d'aretes avec la methode de kronecker.
 * \param int : Le log2 du nombre de sommet.
 * \param int64_t : Le nombre d'aretes voulues.
 * \param uint64_t : Une graine pour la generation aleatoire.
 * \param uint64_t : L'autre graine pour la generation aleatoire.
 * \param int64_t* : Le nombre d'aretes créées.
 * \param packed_edge** : Les aretes genere.
 */
void make_graph(int, int64_t, uint64_t, uint64_t, int64_t*, packed_edge**);

#endif
