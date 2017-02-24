#include "tree.h"

using namespace std;

Tree::Tree()
    : _root()
{
    #ifdef DEBUG_0
    cout << "[Tree][Tree] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Tree][Tree] Done" << endl;
    #endif
}

Tree::Tree(const Tree&)
{
    #ifdef DEBUG_0
    cout << "[Tree][Tree(const Tree&)] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Tree][Tree(const Tree&)] Done" << endl;
    #endif
}

Tree::~Tree()
{
    #ifdef DEBUG_0
    cout << "[Tree][~Tree] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Tree][~Tree] Done" << endl;
    #endif
}
