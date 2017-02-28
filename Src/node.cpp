#include "node.h"

using namespace std;

Node::Node()
    : _childs()
{
    #ifdef DEBUG_OUTPUT
    cout << "[Node][Node] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[Node][Node] Done" << endl;
    #endif
}

Node::~Node()
{
    #ifdef DEBUG_OUTPUT
    cout << "[Node][~Node] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[Node][~Node] Done" << endl;
    #endif
}
