#include <math.h>
#include "graph.h"

using namespace std;

Graph::Graph()
    : _nNode(0),
      _nEdge(0),
      _adj(),
      _trees()
{
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][Graph] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][Graph] Done" << endl;
    #endif
}

Graph::Graph(const Graph&)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][Graph(const Graph&)] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][Graph(const Graph&)] Done" << endl;
    #endif
}

Graph::~Graph()
{
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][~Graph] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[Graph][~Graph] Done" << endl;
    #endif
}

ostream& operator<<(ostream& flux, const Graph& g)
{
	int unsigned i=0;
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
    Matrix<unsigned> ij(standard,2,m,1);
    double ab = A_PROB + B_PROB;
    double c_norm = C_PROB / (1-(ab));
    double a_norm = A_PROB / ab;
    for(unsigned ib=0 ; ib<scale ; ib++)
    {
		Matrix<double> ii_bit(stdrand,1,m);
        ii_bit.superior(ab);

        Matrix<double> ij_bit(stdrand,1,m);
        ij_bit.superior(c_norm*ii_bit + a_norm * !ii_bit);

        Matrix<unsigned> temp(standard,2,m);
        for(unsigned i=0 ; i<m ; i++)
        {
            temp[0][i] = (unsigned)ii_bit[0][i];
            temp[1][i] = (unsigned)ij_bit[0][i];
        }

        ij.add(temp.mutiply(pow(2,ib)));
    }
    cout << endl;
    cout << ij << endl;
    Matrix<int> p(randperm,1,n);
    for(int i=0 ; i<ij.getHeight() ; i++)
        for(int j=0 ; j<ij.getWidht() ; j++)
            ij[i][j] = p[0][ij[i][j]-1];
    cout << endl;
    cout << p << endl;
    cout << endl;
    cout << ij << endl;
    cout << endl;

    p = Matrix<int>(randperm,1,m);
    Matrix<int> res(standard,2,m);
    for(int j=0 ; j<ij.getWidht() ; j++)
    {
        res[0][j] = ij[0][p[0][j]];
        res[1][j] = ij[1][p[0][j]];
    }
    cout << endl;
    cout << p << endl;
    cout << endl;
    cout << res << endl;
}
