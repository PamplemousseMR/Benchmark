#include "node.h"

using namespace std;

Node::Node()
    : _childs()
{
    #ifdef DEBUG_0
    cout << "[Node][Node] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Node][Node] Done" << endl;
    #endif
}

Node::Node(const Node&)
{
    #ifdef DEBUG_0
    cout << "[Node][Node(const Node&)] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Node][Node(const Node&)] Done" << endl;
    #endif
}

Node::~Node()
{
    #ifdef DEBUG_0
    cout << "[Node][~Node] ..." << endl;
    #endif
    #ifdef DEBUG_0
    cout << "[Node][~Node] Done" << endl;
    #endif
}
