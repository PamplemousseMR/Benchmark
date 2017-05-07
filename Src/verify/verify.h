#ifndef __VERIFY__
#define __VERIFY__

#include "../compat.h"
#include <stdio.h>				/*	sprintf	*/
#include <stdint.h>				/*	int64_t	*/
#include <assert.h>				/*	assert	*/

#include "../edge_struct.h"		/*	packed_edge	*/
#include "../xalloc.h"			/*	xmalloc	*/
#include "../options.h"			/* VERBOSE */

/*	=============== Defines ===============	*/

#ifdef GRAPH_VERIFY_OCL
#include <CL/cl.h>				/*	openCL */
#include "../opencl.h"

#define OPTIMAL_VERIFY_MOD 32
#define VERIFY_ITEMS_BY_GROUP 32
#endif

#ifdef GRAPH_VERIFY_OMP
	#ifdef _WIN32
		#define VERIFY_OMP(x) __pragma(x)
	#else
		#define VERIFY_OMP(x) _Pragma(#x)
	#endif

	#include <omp.h>

#else
	#define	VERIFY_OMP(X)
#endif

#ifdef OPENCL_GPU
	#define VERIFY_DEVICE_TYPE CL_DEVICE_TYPE_GPU
#elif OPENCL_CPU
	#define VERIFY_DEVICE_TYPE CL_DEVICE_TYPE_CPU
#elif OPENCL_ACCELERATOR
	#define VERIFY_DEVICE_TYPE CL_DEVICE_TYPE_ACCELERATOR
#else
	#define VERIFY_DEVICE_TYPE CL_DEVICE_TYPE_ALL
#endif

/*	=============== Includes ===============	*/

#ifdef GRAPH_VERIFY_OCL
	#include <CL/cl.h>      /* OpenCL */
	#include <limits.h>     /* max */
	#include <math.h>		/* ceil */
#endif

/*	=============== Functions ===============	*/

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
