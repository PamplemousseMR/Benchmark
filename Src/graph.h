#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include "tree.h"

class Graph
{
    private :

        int _nNode;
        std::vector<std::vector<int> > _adj;
        std::vector<Tree> _trees;

    public :

        Graph();
        Graph(const Graph&);
        ~Graph();
};

#endif // GRAPH_H
