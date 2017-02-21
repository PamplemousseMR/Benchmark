#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "node.h"

class Tree
{
    private :

        Node _root;

    public :

        Tree();
        Tree(const Tree&);
        ~Tree();
};

#endif // TREE_H
