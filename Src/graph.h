#ifndef GRAPH_H
#define GRAPH_H
/*!
 * \file graph.h
 * \brief contient la classe representant le lecteur
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
#include <vector>
#include <iostream>
#include "tree.h"
/*!
 * \class Graph
 * \brief classe representant un graphe
 */
class Graph
{
    private :
        /*!
         * \brief _nNode nombre de sommet du graphe
         */
        int _nNode;
        /*!
         * \brief _adj liste d'adjacence
         */
        std::vector<std::vector<int> > _adj;
        /*!
         * \brief _trees arbres relatifs au graphe
         */
        std::vector<Tree> _trees;
    public :
        /*!
         * \brief Graph constructeur
         */
        Graph();
        /*!
         * \brief Graph constructeur par copie
         */
        Graph(const Graph&);
        /*!
         * \brief ~Graph destructeur
         */
        ~Graph();
};

#endif // GRAPH_H
