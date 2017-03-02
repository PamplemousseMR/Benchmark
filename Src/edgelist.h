#ifndef EdgeList_H
#define EdgeList_H
/*!
 * \file EdgeList.h
 * \brief contient la classe representant une liste d'aretes
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */
#include <iostream>     //cout
#include <stdexcept>    //length_error
#include <list>         //list
#include <typeinfo>     //typeid
#include "commun.h"
/*!
 * \class EdgeList<T>
 * \brief classe representant une liste d'aretes
 */
template<typename T>
class EdgeList
{
    private :

        std::list<T> _first;
        std::list<T> _second;

    public :
        /*!
         * \fn EdgeList()
         * \brief Constructeu
         * \param unsigned : taille de la liste
         * \param T valeur de remplissage
         */
        EdgeList(unsigned, T=0);
        /*!
         * \fn EdgeList(const EdgeList&)
         * \brief Constructeur par copie
         * \param EdgeList<T> La matrice a copier
         */
        EdgeList(const EdgeList&);
        /*!
         * \fn ~EdgeList()
         * \brief Destructeur
         */
        ~EdgeList();
        /*!
         * \fn size_t getWidth() const
         * \brief Retourne la taille de la liste
         * \return size_t : La largeur
         */
        size_t getWidth() const;
        /*!
         * \fn EdgeList& add(const EdgeList<E>&) throw(thr)
         * \brief Modifie la liste en fonction d'une autre liste avec l'operateur +
         * \param EdgeList La liste a additionner
         * \return EdgeList<T> La liste modifiee
         * \throws length_error si la taille des liste sont differentes
         */
        template<typename E>
        #ifdef DEBUG_EXCEPTION
        EdgeList& add(const EdgeList<E>&) throw(thr);
        #else
        EdgeList& add(const EdgeList<E>&);
        #endif
        /*!
         * \fn EdgeList& mutiply(double)
         * \brief Multiplie la liste par la valeur
         * \param double : la valeur a multiplier
         * \return EdgeList<T> la liste modifiee
         */
        EdgeList& mutiply(double);
        /*!
         * \fn EdgeList& operator=(const EdgeList&)
         * \brief Surcharge de l'opérateur =
         * \param EdgeList<T> la liste a copier
         * \return EdgeList<T> la liste modifiee
         */
        EdgeList& operator=(const EdgeList<T>&);
        /*!
         * \fn friend std::ostream& operator<<(std::ostream& flux, const EdgeList<T>& m)
         * \brief Permet l'affichage d'une liste d'aretes
         * \param ostream : Le flux
         * \param EdgeList<T> La liste a afficher
         * \return ostream : Le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream& flux, const EdgeList<T>& m)
        {
            for(auto i=m._first.begin() ; i!=m._first.end() ; ++i)
                flux << *i << ", ";
            flux << std::endl;
            for(auto i=m._second.begin() ; i!=m._second.end() ; ++i)
                flux << *i << ", ";
            return flux;
        }
};

using namespace std;

template<typename T>
EdgeList<T>::EdgeList(unsigned width, T val)
    : _first(width),
      _second(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][EdgeList(unsigned, T)] ..." << endl;
    #endif

    //initialisation
    for(auto i=_first.begin(), j=_second.begin(); i!=_first.end() ; ++i, ++j)
    {
        *i = val;
        *j = val;
    }

    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][EdgeList(unsigned, T)] Done" << endl;
    #endif
}

template<typename T>
EdgeList<T>::EdgeList(const EdgeList<T>& EdgeList)
    : _first(EdgeList.getWidth()),
      _second(EdgeList.getWidth())
{
    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][EdgeList(const EdgeList&)] ..." << endl;
    #endif

    auto i = _first.begin();
    auto j=_second.begin();
    auto mi=EdgeList._first.begin();
    auto mj=EdgeList._second.begin();
    //initialisation
    for(; i!=_first.end() ; ++i, ++j, ++mi, ++mj)
    {
        *i = *mi;
        *j = *mj;
    }

    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][EdgeList(const EdgeList&)] Done" << endl;
    #endif
}

template<typename T>
EdgeList<T>::~EdgeList()
{
    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][~EdgeList()] ..." << endl;
    #endif
    #ifdef DEBUG_OUTPUT
    cout << "[EdgeList<" << typeid(T).name() << ">][~EdgeList()] Done" << endl;
    #endif
}


template<typename T>
size_t EdgeList<T>::getWidth() const
{
    return _first.size();
}

template<typename T>
EdgeList<T>& EdgeList<T>::operator=(const EdgeList<T>& m)
{
    auto mi=EdgeList._first.begin();
    auto mj=EdgeList._second.begin();
    //si la matrice est differante en taille
    if(_width!=m.getWidth())
    {
        _first = list<T>(m.getWidth());
        _second = list<T>(m.getWidth());
        auto i = _first.begin();
        auto j=_second.begin();
        //initialisation
        for(; i!=_first.end() ; ++i, ++j, ++mi, ++mj)
        {
            *i = *mi;
            *j = *mj;
        }
    }
    else
    {
        auto i = _first.begin();
        auto j=_second.begin();
        //initialisation
        for(; i!=_first.end() ; ++i, ++j, ++mi, ++mj)
        {
            *i = *mi;
            *j = *mj;
        }
    }
    return *this;
}

template<typename T>
template<typename E>
#ifdef DEBUG_EXCEPTION
EdgeList<T>& EdgeList<T>::add(const EdgeList<E>& m) throw(thr)
#else
EdgeList<T>& EdgeList<T>::add(const EdgeList<E>& m)
#endif
{
    #ifdef DEBUG_EXCEPTION
    if(_first.size()!=m.getWidth())
    {
        string ex = "[EdgeList<";
        ex += typeid(T).name();
        ex += ">][add] index out of bound";
        throw length_error(ex.c_str());
    }
    #endif
    auto i = _first.begin();
    auto j=_second.begin();
    auto mi=m._first.begin();
    auto mj=m._second.begin();
    //initialisation
    for(; i!=_first.end() ; ++i, ++j, ++mi, ++mj)
    {
        *i = *i + *mi;
        *j = *j + *mj;
    }
    return *this;
}

template<typename T>
EdgeList<T>& EdgeList<T>::mutiply(double val)
{
    auto i = _first.begin();
    auto j=_second.begin();
    //initialisation
    for(; i!=_first.end() ; ++i, ++j)
    {
        *i = (T)(*i * val);
        *j = (T)(*j * val);
    }
    return *this;
}
#endif // EdgeList_H
