#ifndef TREE_H
#define TREE_H
/*!
 * \file tree.h
 * \brief Contient la classe representant un arbre
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
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

        Node _root;/*!< Racine de l'arbre */

    public :
        /*!
         * \fn Tree()
         * \brief Constructeur par defaut
         */
        Tree();
        /*!
         * \fn Tree(const Tree&)
         * \brief Constructeur par copie
         * \param Tree : l'arbre à copier
         */
        Tree(const Tree&);
        /*!
         * \fn ~Graph()
         * \brief Destructeur
         */
        ~Tree();
};

#endif // TREE_H
