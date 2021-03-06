#pragma once

/*	=============== Includes ===============	*/

#include <inttypes.h>	/* int64_t */

#ifdef __cplusplus
extern "C" {
#endif

/*	=============== Struct ===============	*/

/*!
 * \struct packed_edge
 * \brief represente une arete en memoire.
 */
typedef struct packed_edge {
  int64_t v0;   /*!< Premiere valeur de l'arete. */
  int64_t v1;   /*!< Second valeur de l'arete. */
} packed_edge;

/*	=============== Functions ===============	*/

/*!
 * \fn static __inline int64_t get_v0_from_edge(const packed_edge* p)
 * \brief Retourne la premiere valeur d'un arete.
 * \param packed_edge* : l'arete.
 * \return int64 : la premiere valeur de l'arete.
 */
static __inline int64_t get_v0_from_edge(const packed_edge*);
/*!
 * \fn static __inline int64_t get_v1_from_edge(const packed_edge* p)
 * \brief Retourne la second valeur d'un arete.
 * \param packed_edge* : l'arete.
 * \return int64 : la second valeur de l'arete.
 */
static __inline int64_t get_v1_from_edge(const packed_edge*);

/*	=============== Inline definitions ===============	*/

static __inline int64_t get_v0_from_edge(const packed_edge* p) {
  return p->v0;
}

static __inline int64_t get_v1_from_edge(const packed_edge* p) {
  return p->v1;
}

#ifdef __cplusplus
}
#endif
