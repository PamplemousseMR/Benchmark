#ifndef NODE_H
#define NODE_H
/*!
 * \file node.h
 * \brief Contient la classe representant un noeud d'un arbre
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi */
#include <vector>
#include <iostream>
/*!
 * \class Node
 * \brief classe representant un noeud d'un arbre
 */
class Node;
class Node
{
    private :
        std::vector<Node*> _childs;/*!< Enfants du noeud */
    public :
        /*!
         * fn Node()
         * \brief Constructeur par defaut
         */
        Node();
        /*!
         * \fn Node(const Node&)
         * \brief Constructeur par copie
         * \param Node : le noeud à copier
         */
        Node(const Node&);
        /*!
         * \fn ~Node()
         * \brief Destructeur
         */
        ~Node();
};

#endif // NODE_H
