#pragma once

/*	=============== Includes ===============	*/

#include <stdint.h>		/*	uint64_t	*/
#include <stdlib.h>		/*	NULL */
#include <errno.h>		/*	errno	*/
#include "mod_arith.h"	/*	mod_*	*/

#ifdef __cplusplus
extern "C" {
#endif

/*	=============== Defines ===============	*/

#define	MRG_RAND_MAX 2147483647

/*	=============== Struct ===============	*/

/*!
 * \struct mrg_state
 * \brief Permet de representer une des graines.
 */
typedef struct mrg_state {
  uint_fast32_t z1, z2, z3, z4, z5;
} mrg_state;
/*!
 * \struct mrg_transition_matrix
 * \brief Matrice de transition pour passer ou prochain nombre.
 */
typedef struct mrg_transition_matrix {
    uint_fast32_t s, t, u, v, w;
    uint_fast32_t a, b, c, d;
} mrg_transition_matrix;

/*	=============== Global ===============	*/

/*!
 * \brief Graine pour la generation aleatoire.
 */
extern uint64_t userseed;
/*!
 * \brief Utiliser pour la recuperation d'une valeur aleatoire.
 */
extern void* prng_state;

/*	=============== Functions ===============	*/

/*!
 * \fn uint_fast32_t mrg_get_uint_orig(mrg_state*)
 * \brief retourne un int aleatoire [0, 2^31-1)
 * \param mrg_state* : La graine.
 * \return uint_fast32_t : Le int generer.
 */
uint_fast32_t mrg_get_uint_orig(mrg_state*);
/*!
 * \fn double mrg_get_double_orig(mrg_state*)
 * \brief retourne un double aleatoire [0, 1)
 * \param mrg_state* : La graine.
 * \return double : Le double generer.
 */
double mrg_get_double_orig(mrg_state*);
/*!
 * \fn void make_mrg_seed(uint64_t, uint64_t, uint_fast32_t*)
 * \brief Genere une graine pour la generation de nombre aleatoire.
 * \param userseed1 : Une premiere graine.
 * \param userseed2 : Une deuxieme graine.
 * \param seed : La graine generer.
 */
void make_mrg_seed(uint64_t, uint64_t, uint_fast32_t[5]);
/*!
 * \fn void init_random()
 * \brief Genere la graine userseed et prng_state.
 */
void init_random();

#ifdef __cplusplus
}
#endif
