#include <math.h>
#include "graph.h"

using namespace std;

Graph::Graph()
    : _nNode(0),
      _nEdge(0),
      _edges(NULL),
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

    if(_edges != NULL)
        delete _edges;

    #ifdef DEBUG_OUTPUT
    cout << "[Graph][~Graph] Done" << endl;
    #endif
}

ostream& operator<<(ostream& flux, const Graph& g)
{
	int unsigned i=0;
    for(vector<unsigned> vec : g._adj)
    {
       flux << i << " -> ";
       for(unsigned val : vec)
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

        // dimensions du graphe
        _nNode = 1 << scale;
        _nEdge = _nNode*edgeFactor;

        // valeurs dépendantes des probabilités et utilisées lors de la génération
        double ab = A_PROB + B_PROB;
        double c_norm = C_PROB / (1-ab);
        double a_norm = A_PROB / ab;

        // variables de parcours
        unsigned h = 0;
        unsigned w = 1;

        // création de la matrice d'adjacence
        if(_edges != NULL)
            delete _edges;
        _edges = new Matrix<unsigned>(2,_nEdge);

        // matrices intermédiaires de calcul
        Matrix<unsigned> ij(standard,2,_nEdge,1);
        Matrix<double> ii_bit(1,_nEdge);
        Matrix<double> ij_bit(1,_nEdge);

        // boucle sur la puissance de 2 utilisée lors de la génération
        unsigned int ib=-1;
        while(++ib<scale)
        {
            cout << ib+1 << "/" << scale << endl;

            // construction de ii_bit : 1 si la valeur générée est supérieur à A+B
            ii_bit.generate(stdrand);
            ii_bit.superior(ab);
            Matrix<double> mul(ii_bit);

            // remplissage de la matrice d'adjacences
            h = -1;
            while(++h < _nEdge)
                (*_edges)[0][h] = (unsigned)ii_bit[0][h];

            // construction de ij_bit : 1 si la valeur générée est supérieure à la valeur
            // correspondante dans ii_bit (modifiée avec les valeurs de probabilités)
            ij_bit.generate(stdrand);
            mul.mutiply(c_norm);
            ij_bit.superior(ii_bit.no().mutiply(a_norm).add(mul));
            // remplissage de la matrice d'adjacences
            h = -1;
            while(++h < _nEdge)
                (*_edges)[1][h] = (unsigned)ij_bit[0][h];

            ij.add(_edges->mutiply(w));
            w <<= 1;
        }
        cout << "end" << endl;

        // première permutation sur les sommets
        Matrix<int> p(randperm,1,_nNode);
        h = -1;
        while(++h < ij.getHeight())
        {
            w = -1;
            while(++w < ij.getWidth())
                ij[h][w] = p[0][ij[h][w]-1];
        }

        // seconde permutation sur les arêtes
        p = Matrix<int>(randperm,1,_nEdge);
        h = -1;
        while(++h < ij.getWidth())
        {
            (*_edges)[0][h] = ij[0][p[0][h]];
            (*_edges)[1][h] = ij[1][p[0][h]];
        }

        // affichage de la matrice d'adjacences
        cout << (*_edges) << endl;
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
