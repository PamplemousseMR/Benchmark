#ifndef NODE_H
#define NODE_H
/*!
 * \file node.h
<<<<<<< HEAD
 * \brief Contient la classe représentant un noeud d'un arbre
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */
#include <vector>
#include <iostream>
/*!
 * \class Node
 * \brief classe représentant un noeud d'un arbre
 */
class Node;
class Node
{
    private :

        std::vector<Node*> _childs;     /*!< Enfants du noeud */

    public :
        /*!
         * \fn Node()
         * \brief Constructeur par defaut
         */
        Node();
        /*!
         * \fn ~Node()
         * \brief Destructeur
         */
        ~Node();
};

#endif // NODE_H
