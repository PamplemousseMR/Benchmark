#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>

class Node;
class Node
{
    private :

        std::vector<Node*> _childs;

    public :

        Node();
        Node(const Node&);
        ~Node();
};

#endif // NODE_H
