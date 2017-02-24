#ifndef GRAPH_H
#define GRAPH_H
/*!
 * \file graph.h
 * \brief contient la classe representant un graphe
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
#include <vector>
#include <iostream>
#include "tree.h"
#include "matrix.h"
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
         * \brief _nEdge nobre d'arrete
         */
        int _nEdge;
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
         * \param int : log2 du nombre de sommet
         * \param int : ratio d'arrete du graphe
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
        /*!
         * \brief generateKroneckerEdges genere les arretes avec la methode de kronecker
         * \param int : log2 des arretes
         * \param int : ratio des arretes du graphe
         */
        void generateKroneckerEdges(unsigned, unsigned);
        /*!
         * \brief operator << permet l'affichage d'un graphe
         * \return le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream&, const Graph&);
};

#endif // GRAPH_H
