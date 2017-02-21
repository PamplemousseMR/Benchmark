#ifndef TREE_H
#define TREE_H
/*!
 * \file tree.h
 * \brief contient la classe representant un arbre
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
#include <iostream>
#include "node.h"
/*!
 * \class Tree
 * \brief classe representant un arbre
 */
class Tree
{
    private :
        /*!
         * \brief _root racine de l'arbre
         */
        Node _root;
    public :
        /*!
         * \brief Tree::Tree Constructeur
         */
        Tree();
        /*!
         * \brief Tree::Tree Constructeur par copie
         */
        Tree(const Tree&);
        /*!
         * \brief Tree::Tree Destructeur
         */
        ~Tree();
};

#endif // TREE_H
