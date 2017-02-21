#ifndef NODE_H
#define NODE_H
/*!
 * \file node.h
 * \brief contient la classe representant un noeud d'un arbre
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
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
        /*!
         * \brief _childs les enfants du noeud
         */
        std::vector<Node*> _childs;
    public :
        /*!
         * \brief Node constructeur
         */
        Node();
        /*!
         * \brief Node constructeur par copie
         */
        Node(const Node&);
        /*!
         * \brief ~Node destructeur
         */
        ~Node();
};

#endif // NODE_H
