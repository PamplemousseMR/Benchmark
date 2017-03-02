#ifndef LIST_H
#define LIST_H

#include <list>     //list
#include <random>   //rand

template<typename T>
class List
{
    private:

        std::list<T> _list;

    public:

        List(unsigned);
        List(const List&);
        ~List();
        void rand();
        void randperm();
        List& superior(double);
        template<typename E>
        List& superior(const List<E>&);
        List& multiply(double);
        template<typename E>
        List& add(const List<E>&);
        List& no();
        List& operator=(const List&);

};

using namespace std;

template<typename T>
List<T>::List(unsigned size)
    : _list(size)
{
    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][List(unsigned)] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][List(unsigned)] Done" << endl;
    #endif
}

template<typename T>
List<T>::List(const List& list)
    : _list(list.size())
{
    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][List(const List&)] ..." << endl;
    #endif

    for(auto i=_list.begin(), j=list.begin(); i!=_list.end() ; ++i, j++)
        *i = *j;

    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][List(const List&)] Done" << endl;
    #endif
}

template<typename T>
List<T>::~List()
{
    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][~List()] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[List<" << typeid(T).name() << ">][~List()] Done" << endl;
    #endif
}

template<typename T>
void List<T>::rand()
{
    for(auto i=_list.begin(); i!=_list.end() ; ++i)
        *i = (T)rand();
}

template<typename T>
void List<T>::randperm()
{

}

template<typename T>
List<T>& List<T>::superior(double val)
{
    for(auto i=_list.begin(); i!=_list.end() ; ++i)
        *i = (T)(*i > val);
    return *this;
}

template<typename T>
template<typename E>
List<T>& List<T>::superior(const List<E>& list)
{
    for(auto i=_list.begin(), j=list.begin(); i!=_list.end() ; ++i, j++)
        *i = (T)(*i > *j);
    return *this;
}

template<typename T>
List<T>& List<T>::multiply(double val)
{
    for(auto i=_list.begin(); i!=_list.end() ; ++i)
        *i = (T)(*i * val);
    return *this;
}

template<typename T>
template<typename E>
List<T>& List<T>::add(const List<E>& list)
{
    for(auto i=_list.begin(), j=list.begin(); i!=_list.end() ; ++i, j++)
        *i = (T)(*i + *j);
    return *this;
}

template<typename T>
List<T>& List<T>::no()
{
    for(auto i=_list.begin(); i!=_list.end() ; ++i)
        *i = !*i;
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>&)
{
    for(auto i=_list.begin(), j=list.begin(); i!=_list.end() ; ++i, j++)
        *i = *j;
    return *this;
}

#endif // LIST_H
