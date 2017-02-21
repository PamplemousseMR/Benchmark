#include "node.h"

using namespace std;

Node::Node()
    : _childs()
{
    #ifdef DEBUG
    cout << "[Node][Node] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Node][Node] Done" << endl;
    #endif
}

Node::Node(const Node&)
{
    #ifdef DEBUG
    cout << "[Node][Node(const Node&)] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Node][Node(const Node&)] Done" << endl;
    #endif
}

Node::~Node()
{
    #ifdef DEBUG
    cout << "[Node][~Node] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Node][~Node] Done" << endl;
    #endif
}
