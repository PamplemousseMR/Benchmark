#include "graph.h"

using namespace std;

Graph::Graph()
    : _nNode(0),
      _adj(),
      _trees()
{
    #ifdef DEBUG
    cout << "[Graph][Graph] ..." << endl;
    #endif
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
