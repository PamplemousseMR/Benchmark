#ifndef __VERIFY__
#define __VERIFY__

#include "../compat.h"
#include <stdio.h>				/*	sprintf	*/
#include <stdint.h>				/*	int64_t	*/
#include <assert.h>				/*	assert	*/

#include "../edge_struct.h"		/*	packed_edge	*/
#include "../xalloc.h"			/*	xmalloc	*/

/*!
 * \fn int64_t verify_bfs_tree (int64_t*, int64_t ,int64_t , const packed_edge*, int64_t)
 * \brief Verifi qu'un parcours en largeur est correcte.
 * \param int64_t* : La liste des parent de chaque sommet apres le bfs
 * \param int64_t :
 * \param int64_t
 * \param
 * \param
 * \return
 */
int64_t verify_bfs_tree (int64_t*, int64_t ,int64_t , const packed_edge*, int64_t);

#endif
