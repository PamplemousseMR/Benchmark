#include "graph.h"

using namespace std;

Graph::Graph(int nNode)
    : _nNode(nNode),
      _adj(),
      _trees()
{
    #ifdef DEBUG
    cout << "[Graph][Graph] ..." << endl;
    #endif

    for (int i = 0; i < nNode; i++)
        _adj.push_back(vector<int>());

    #ifdef DEBUG
    cout << "[Graph][Graph] Done" << endl;
    #endif
}

Graph::Graph(const Graph&)
{
    #ifdef DEBUG
    cout << "[Graph][Graph(const Graph&)] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Graph][Graph(const Graph&)] Done" << endl;
    #endif
}

Graph::~Graph()
{
    #ifdef DEBUG
    cout << "[Graph][~Graph] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Graph][~Graph] Done" << endl;
    #endif
}
