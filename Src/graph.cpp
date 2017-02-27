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

#ifdef DEBUG_EXCEPTION
void Graph::generateKroneckerEdges(unsigned scale, unsigned edgeFactor) throw (thr)
#else
void Graph::generateKroneckerEdges(unsigned scale, unsigned edgeFactor)
#endif
{
    #ifdef DEBUG_EXCEPTION
    try
    {
    #endif
        unsigned n = (unsigned)pow(2,scale);
        unsigned m = n*edgeFactor;
        double ab = A_PROB + B_PROB;
        double c_norm = C_PROB / (1-(ab));
        double a_norm = A_PROB / ab;
        unsigned h = -1;
        unsigned w = -1;

        Matrix<unsigned> ij(standard,2,m,1);
        Matrix<unsigned> temp(2,m);
        Matrix<double> ii_bit(1,m);
        Matrix<double> ij_bit(1,m);

        unsigned int ib=-1;
        while(++ib<scale)
        {
            cout << ib+1 << "/" << scale << endl;
            ii_bit.generate(stdrand);
            ii_bit.superior(ab);
            Matrix<double> mul(ii_bit);

            h = -1;
            while(++h < m)
                temp[0][h] = (unsigned)ii_bit[0][h];

            ij_bit.generate(stdrand);
            mul.mutiply(c_norm);
            ij_bit.superior(ii_bit.no().mutiply(a_norm).add(mul));

            h = -1;
            while(++h < m)
                temp[1][h] = (unsigned)ij_bit[0][h];

            ij.add(temp.mutiply(pow(2,ib)));
        }
        cout << "end" << endl;

        Matrix<int> p(randperm,1,n);
        h = -1;
        while(++h < ij.getHeight())
        {
            w = -1;
            while(--w < ij.getWidth())
                ij[h][w] = p[0][ij[h][w]-1];
        }

        p = Matrix<int>(randperm,1,m);
        h = -1;
        while(++h < ij.getWidth())
        {
            temp[0][h] = ij[0][p[0][h]];
            temp[1][h] = ij[1][p[0][h]];
        }

        cout << temp << endl;
    #ifdef DEBUG_EXCEPTION
    }
    catch(exception e)
    {
        string ex("[Graph][generateKroneckerEdges] ");
        ex +=  string(e.what());
        throw length_error(ex.c_str());
    }
    #endif
}
