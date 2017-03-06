#ifndef __XALLOC__
#define __XALLOC__

/*!
 * \file xalloc.h
 * \param Fichier contenent des fonction permetant de faire des allocations dynamiques.
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */

/*	=============== Includes ===============	*/

#include "compat.h"
#include <stdlib.h>		/*	malloc	*/
#include <stdio.h>		/*	print	*/
#ifdef _WIN32
#include <windows.h>
#endif

/*	=============== Functions ===============	*/

void* xmalloc(size_t);
void* xmalloc_large(size_t);
void* xmalloc_large_ext(size_t);
void xfree_large(void*);

#endif
