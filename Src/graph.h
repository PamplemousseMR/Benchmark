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
#include "commun.h"
#include "tree.h"
#include "matrix.h"
/*!
 * \class Graph
 * \brief classe representant un graphe
 */
class Graph
{
    private :

        int _nNode;/*!< Nombre de sommet */
        int _nEdge;/*!< Nombre d'arrete */
        std::vector<std::vector<int> > _adj;/*!< Liste d'adjacence */
        std::vector<Tree> _trees;/*!< Arbres relatifs au graphe */

    public :
        /*!
         * \fn Graph()
         * \brief constructeur par default
         */
        Graph();
        /*!
         * \fn Graph(const Graph&)
         * \brief constructeur par copie
         * \param Graph : le graphe a copier
         */
        Graph(const Graph&);
        /*!
         * \fn ~Graph()
         * \brief destructeur
         */
        ~Graph();
        /*!
         * \fn void generateKroneckerEdges(unsigned, unsigned)
         * \brief genere les arretes avec la methode de kronecker
         * \param int : log2 des arretes
         * \param int : ratio des arretes du graphe
         */
        void generateKroneckerEdges(unsigned, unsigned);
        /*!
         * \fn friend std::ostream& operator<<(std::ostream&, const Graph&)
         * \brief permet l'affichage d'un graphe
         * \param ostream : le flux
         * \param Graph : le graphe
         * \return le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream&, const Graph&);
};

#endif // GRAPH_H
