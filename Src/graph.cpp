#include "graph.h"

using namespace std;

Graph::Graph()
    : _nNode(0),
      _nEdge(0),
      _adj(),
      _trees()
{
    #ifdef DEBUG_0
    cout << "[Graph][Graph] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Graph][Graph] Done" << endl;
    #endif
}

Graph::Graph(const Graph&)
{
    #ifdef DEBUG_0
    cout << "[Graph][Graph(const Graph&)] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Graph][Graph(const Graph&)] Done" << endl;
    #endif
}

Graph::~Graph()
{
    #ifdef DEBUG_0
    cout << "[Graph][~Graph] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Graph][~Graph] Done" << endl;
    #endif
}

ostream& operator<<(ostream& flux, const Graph& g)
{
    int i=0;
    for(vector<int> vec : g._adj)
    {
       flux << i << " -> ";
       for(int val : vec)
           flux << val << ", ";
       if(i < g._adj.size()-1)
           flux << endl;
       i++;
    }
    return flux;
}

void Graph::generateKroneckerEdges(unsigned scale, unsigned edgeFactor)
{
    unsigned n = (unsigned)pow(2,scale);
    unsigned m = n*edgeFactor;
    double a = 0.57;
    double b = 0.19;
    double c = 0.19;
    Matrix<unsigned> ij(2,m,1);
    double ab = a+b;
    double c_norm = c/(1-(ab));
    double a_norm = a/ab;
    cout << ij << endl;
    for(unsigned ib=0 ; ib<scale ; ib++)
    {
        Matrix<double> ii_bit(1,m);
        ii_bit.superior(ab);

        Matrix<double> ij_bit(1,m);
        ij_bit.superior(c_norm*ii_bit + a_norm * !ii_bit);

        Matrix<unsigned> temp(2,m,0);
        for(unsigned i=0 ; i<m ; i++)
        {
            temp[0][i] = (unsigned)ii_bit[0][i];
            temp[1][i] = (unsigned)ij_bit[0][i];
        }
        ij.add(temp.mutiply(pow(2,ib)));
    }
    cout << endl;
    cout << ij << endl;
}
