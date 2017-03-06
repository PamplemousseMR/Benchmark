#ifndef __EDGE_STRUCT__
#define __EDGE_STRUCT__
/*!
 * \file edge_struct.h
 * \param fichier qui englobe le systeme de representation des aretes
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */
#include <inttypes.h>	/* int64_t */

#ifdef __cplusplus
extern "C" {
#endif

/*	===== Struct =====	*/

/*!
 * \struct packed_edge
 * \brief represente une arete en memoire
 */
typedef struct packed_edge {
  int64_t v0;
  int64_t v1;
} packed_edge;

/*	===== Functions =====	*/

/*!
 * \fn static __inline int64_t get_v0_from_edge(const packed_edge* p)
 * \brief retourne la premiere valeur d'un arete
 * \param packed_edge* : l'arete
 * \return int64 : la premiere valeur de l'arete
 */
static __inline int64_t get_v0_from_edge(const packed_edge* p) {
  return p->v0;
}
/*!
 * \fn static __inline int64_t get_v1_from_edge(const packed_edge* p)
 * \brief retourne la second valeur d'un arete
 * \param packed_edge* : l'arete
 * \return int64 : la second valeur de l'arete
 */
static __inline int64_t get_v1_from_edge(const packed_edge* p) {
  return p->v1;
}
/*!
 * \fn static __inline void write_edge(packed_edge* p, int64_t v0, int64_t v1) {
 * \brief remplie l'arete avec les valeurs fournie
 * \param packed_edge* : l'arete
 * \param int64_t : la premiere valeur
 * \param int64_t : la second valeur
 */
static __inline void write_edge(packed_edge* p, int64_t v0, int64_t v1) {
  p->v0 = v0;
  p->v1 = v1;
}

#ifdef __cplusplus
}
#endif

#endif
