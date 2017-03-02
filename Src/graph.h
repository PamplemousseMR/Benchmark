#ifndef GRAPH_H
#define GRAPH_H
/*!
 * \file graph.h
 * \brief contient la classe representant un graphe
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */
#include <vector>
#include <iostream>
#include <stdexcept>
#include "commun.h"
#include "tree.h"
#include "edgelist.h"
/*!
 * \class Graph
 * \brief classe representant un graphe
 */
class Graph
{
    private :

        unsigned _nNode;                             /*!< Nombre de sommet */
        unsigned _nEdge;                             /*!< Nombre d'arête */
        EdgeList<unsigned>* _edges;                 /*!< Liste d'arêtes */
        std::vector<std::vector<unsigned> > _adj;    /*!< Liste d'adjacence */
        std::vector<Tree> _trees;               /*!< Arbres relatifs au graphe */

    public :
        /*!
         * \fn Graph()
         * \brief Constructeur par defaut
         */
        Graph();
        /*!
         * \fn ~Graph()
         * \brief Destructeur
         */
        ~Graph();
        /*!
         * \fn void generateKroneckerEdges(unsigned, unsigned)
         * \brief Génère les arêtes avec la méthode de kronecker
         * \param int : log2 des arêtes
         * \param int : ratio des arêtes du graphe
         */
        #ifdef DEBUG_EXCEPTION
        void generateKroneckerEdges(unsigned, unsigned) throw (thr);
        #else
        void generateKroneckerEdges(unsigned, unsigned);
        #endif
        /*!
         * \fn void generateGraph() throw (thr)
         * \brief Génère le graphe
         */
        #ifdef DEBUG_EXCEPTION
        void generateGraph() throw (thr);
        #else
        void generateGraph();
        #endif
        /*!
         * \fn friend std::ostream& operator<<(std::ostream&, const Graph&)
         * \brief Permet l'affichage d'un graphe
         * \param ostream : le flux
         * \param Graph : le graphe
         * \return le stream passé en parametre modifié
         */
        friend std::ostream& operator<<(std::ostream&, const Graph&);
};

#endif // GRAPH_H
