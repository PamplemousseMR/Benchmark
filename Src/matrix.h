#ifndef MATRIX_H
#define MATRIX_H
/*!
 * \file Matrix.h
 * \brief contient la classe representant une matrice
 * \author Appert Kevin
 * \author Bocahu Florent
 * \author Hun Tony
 * \author Lataix Maxime
 * \author Manciaux Romain
 * \author Peccard Remi
 */
#include <iostream> //cout
#include <stdlib.h> //rand
#include <stdexcept> //length_error
#include <vector> //vector
#include <algorithm> //random_shuffle
#include <random> //default_random_engine
#include <chrono> // chrono::system_clock
#include <typeinfo> //typeid
#include "commun.h"
/*!
 * \enum matrix_type
 * \brief le type de construction de la matrice
 */
enum matrix_type
{
    standard,
	stdrand,
    randperm
};
/*!
 * \class Matrix<T>
 * \brief classe representant une matrice
 */
template<typename T>
class Matrix
{
    private :

        unsigned _height;    /*!< Hauteur de la matrice */
        unsigned _width;     /*!< Largeur de la matrice */
        T** _matrix;    /*!< Les donnees de la matrice */

    public :
        /*!
         * \fn Matrix()
         * \brief Constructeur par default
         * \param unsigned : Hauteur de la matrice
         * \param unsigned : Largeur de la matrice
         */
        Matrix(unsigned, unsigned);
        /*!
         * \fn Matrix(matrix_type,int, int, T=0)
         * \brief Constructeur
         * \param unsigned : Hauteur de la matrice
         * \param unsigned : Largeur de la matrice
         * \param T : Valeur de remplissage
         */
        Matrix(matrix_type,unsigned, unsigned, T=0);
        /*!
         * \fn Matrix(const Matrix&)
         * \brief Constructeur par copie
         * \param Matrix<T> La matrice a copier
         */
        Matrix(const Matrix&);
        /*!
         * \fn ~Matrix()
         * \brief Destructeur
         */
        ~Matrix();
        /*!
         * \fn void generate(matrix_type,int, int, T=0)
         * \brief Regenere la matrice
         * \param int : Hauteur de la matrice
         * \param int : Largeur de la matrice
         * \param T : Valeur de remplissage
         */
        void generate(matrix_type,T=0);
        /*!
         * \fn unsigned getHeight() const
         * \brief Retourne la hauteur de la matrice
         * \return int : La hauteur
         */
        unsigned getHeight() const;
        /*!
         * \fn unsigned getWidth() const
         * \brief Retourne la largeur de la matrice
         * \return int : La largeur
         */
        unsigned getWidth() const;
        /*!
         * \fn Matrix& superior(double)
         * \brief Modifie la matrice en fonction d'une autre matrice avec l'operateur >
         * \param double : La valeur a verifier
         * \return Matrix<T> La matrice courente modifie
         */
        Matrix& superior(double);
        /*!
         * \fn Matrix& superior(const Matrix<E>&) throw(thr)
         * \brief Modifie la matrice en fonction d'une autre matrice avec l'operateur >
         * \param Matrix<T> La matrice a comparer
         * \return Matrix<T> La matrice modifie
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
        #ifdef DEBUG_EXCEPTION
        Matrix& superior(const Matrix<E>&) throw(thr);
        #else
        Matrix& superior(const Matrix<E>&);
        #endif
        /*!
         * \fn Matrix& add(const Matrix<E>&) throw(thr)
         * \brief Modifie la matrice en fonction d'une autre matrice avec l'operateur +
         * \param Matrix Ma matrice a additionner
         * \return Matrix<T> La matrice modifie
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
        #ifdef DEBUG_EXCEPTION
        Matrix& add(const Matrix<E>&) throw(thr);
        #else
        Matrix& add(const Matrix<E>&);
        #endif
        /*!
         * \fn Matrix& mutiply(double)
         * \brief Multiplie la matrice par la valeur
         * \param double : la valeur a multiplier
         * \return Matrix<T> la matrice modifie
         */
        Matrix& mutiply(double);
        /*!
         * \fn Matrix& not()
         * \brief Calcule la matrice oposer (operateur! sur le T)
         * \return Matrix<T> : La matrice modifie
         */
        Matrix& no();
        /*!
         * \fn Matrix& operator=(const Matrix&)
         * \brief Surcharge de l'opérateur =
         * \param Matrix<T> la matrice a copier
         * \return Matrix<T> la matrice modifier
         */
        Matrix& operator=(const Matrix<T>&);
        /*!
         * \fn T* operator[](int) throw(thr)
         * \brief Surcharge de l'opérateur []
         * \param int : L'index de la valeur a recuperer
         * \return T* : tableau d'entier correspondant à la ligne spécifié en paramètres
         */
        #ifdef DEBUG_EXCEPTION
        T* operator[](unsigned) throw(thr);
        #else
        T* operator[](unsigned);
        #endif
        /*!
         * \fn const T* operator[](int) const throw(thr)
         * \brief Surcharge de l'opérateur []
         * \param int : L'index de la valeur a recuperer
         * \return T* : tableau d'entier correspondant à la ligne spécifié en paramètres
         */
        #ifdef DEBUG_EXCEPTION
        const T* operator[](unsigned) const throw(thr);
        #else
        const T* operator[](unsigned) const;
        #endif
        /*!
         * \fn friend std::ostream& operator<<(std::ostream& flux, const Matrix<T>& m)
         * \brief Permet l'affichage d'une matrice
         * \param ostream : Le flux
         * \param Matrix<T> La matrice a afficher
         * \return ostream : Le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream& flux, const Matrix<T>& m)
        {
            unsigned k=0;
            for(unsigned i=0; i<m.getHeight(); ++i)
            {
                for(unsigned j=0; j<m.getWidth(); ++j)
                {
                    flux << m._matrix[i][j];
                    if(j < m.getWidth()-1)
                        flux << ", ";
                }
                if(k < m.getHeight()-1)
                    flux << "\n";
                k++;
            }
            return flux;
        }
};

using namespace std;

template<typename T>
Matrix<T>::Matrix(unsigned height, unsigned width)
    : _height(height),
      _width(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix()] ..." << endl;
    #endif

    //alocation sans initialisation
    _matrix = new T*[_height];
    while(height)
        _matrix[--height] = new T[_width];

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix()] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::Matrix(matrix_type type, unsigned height, unsigned width, T value)
    : _height(height),
      _width(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(matrix_type,int,int," << typeid(T).name() << ")] ..." << endl;
    #endif

    //creation d'un vecteur de taille n contenent n valeurs, puis melange
    vector<int> vec;
    if(type == randperm)
    {
        int i=width;
        while(i)
            vec.push_back(--i);
        #ifdef RANDOM
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
        #else
        auto engine = default_random_engine();
        #endif
		shuffle(begin(vec), end(vec), engine);
    }

    //allocation
    _matrix = new T*[_height];
    while(height)
    {
        _matrix[--height] = new T[_width];
        width = _width;
        while(width)
        {
            --width;
            //affectation
            switch(type)
            {
                case standard:
                    _matrix[height][width] = value;
                break;
				case stdrand:
                    _matrix[height][width] = (T)((double)rand() / (RAND_MAX));
                break;
                case randperm:
                    _matrix[height][width] = (T)vec[width];
                break;
            }
        }
    }

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(matrix_type,int,int," << typeid(T).name() << ")] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& matrix)
    : _height(matrix._height), _width(matrix._width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(const Matrix&)] ..." << endl;
    #endif

    int height = _height;
    int width;

    //allocation et affectation
    _matrix = new T*[_height];
    while(height)
    {
        _matrix[--height] = new T[_width];
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = matrix._matrix[height][width];
        }
    }

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(const Matrix&)] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::~Matrix()
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][~Matrix] ..." << endl;
    #endif

    while(_height)
        delete[] _matrix[--_height];
    delete[] _matrix;

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][~Matrix] Done" << endl;
    #endif
}

template<typename T>
void Matrix<T>::generate(matrix_type type, T value)
{
    vector<int> vec;
    int height = _height;
    int width;

    //creation d'un vecteur de taille n contenent n valeurs, puis melange
    if(type == randperm)
    {
        int i=_width;
        while(i)
            vec.push_back(--i);
        #ifdef RANDOM
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
        #else
        auto engine = default_random_engine();
        #endif
        shuffle(begin(vec), end(vec), engine);
    }

    //affectation
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            switch(type)
            {
                case standard:
                    _matrix[height][width] = value;
                break;
                case stdrand:
                    _matrix[height][width] = (T)((double)rand() / (RAND_MAX));
                break;
                case randperm:
                    _matrix[height][width] = (T)vec[width];
                break;
            }
        }
    }
}

template<typename T>
unsigned Matrix<T>::getHeight() const
{
    return _height;
}

template<typename T>
unsigned Matrix<T>::getWidth() const
{
    return _width;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
    int height = _height;
    int width;

    //si la matrice est differante en taille
    if(_height!=m.getHeight() || _width!=m.getWidth())
    {
        //on supprime la memoire
        while(height)
            delete[] _matrix[--height];
        delete[] _matrix;

        _height = m.getHeight();
        _width = m.getWidth();
        height = _height;

        //on allou et on affecte
        _matrix = new T*[_height];
        while(height)
        {
            _matrix[--height] = new T[_width];
            width = _width;
            while(width)
            {
                --width;
                _matrix[height][width] = m[height][width];
            }
        }
    }
    else
    {
        //on affecte
        while(height)
        {
            --height;
            width = _width;
            while(width)
            {
                --width;
                _matrix[height][width] = m[height][width];
            }
        }
    }
    return *this;
}


template<typename T>
#ifdef DEBUG_EXCEPTION
T* Matrix<T>::operator[](unsigned index) throw(thr)
#else
T* Matrix<T>::operator[](unsigned index)
#endif
{
    #ifdef DEBUG_EXCEPTION
    if(index < 0 || index >= _height)
    {
        string ex = "[Matrix<";
        ex += typeid(T).name();
        ex += ">][operator[]]index out of bound";
        throw length_error(ex.c_str());
    }
    #endif
    return _matrix[index];
}

template<typename T>
#ifdef DEBUG_EXCEPTION
const T* Matrix<T>::operator[](unsigned index) const throw(thr)
#else
const T* Matrix<T>::operator[](unsigned index) const
#endif
{
    #ifdef DEBUG_EXCEPTION
    if(index < 0 || index >= _height)
    {
        string ex = "[Matrix<";
        ex += typeid(T).name();
        ex += ">][operator[]] index out of bound";
        throw length_error(ex.c_str());
    }
    #endif
    return _matrix[index];
}

template<typename T>
Matrix<T>& Matrix<T>::superior(double val)
{
    int height = _height;
    int width;

    //on parcour et on affecte la nouvelle valeur
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = _matrix[height][width]>val;
        }
    }
    return *this;
}

template<typename T>
template<typename E>
#ifdef DEBUG_EXCEPTION
Matrix<T>& Matrix<T>::superior(const Matrix<E>& m) throw(thr)
#else
Matrix<T>& Matrix<T>::superior(const Matrix<E>& m)
#endif
{
    #ifdef DEBUG_EXCEPTION
    if(_height!=m.getHeight() || _width!=m.getWidth())
    {
        string ex = "[Matrix<";
        ex += typeid(T).name();
        ex += ">][superior] index out of bound";
        throw length_error(ex.c_str());
    }
    #endif
    int height = _height;
    int width;

    //on parcour et on affecte la nouvelle valeur
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = _matrix[height][width]>m[height][width];
        }
    }
    return *this;
}

template<typename T>
template<typename E>
#ifdef DEBUG_EXCEPTION
Matrix<T>& Matrix<T>::add(const Matrix<E>& m) throw(thr)
#else
Matrix<T>& Matrix<T>::add(const Matrix<E>& m)
#endif
{
    #ifdef DEBUG_EXCEPTION
    if(_height!=m.getHeight() || _width!=m.getWidth())
    {
        string ex = "[Matrix<";
        ex += typeid(T).name();
        ex += ">][add] index out of bound";
        throw length_error(ex.c_str());
    }
    #endif
    int height = _height;
    int width;

    //on parcour et on affecte la nouvelle valeur
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = _matrix[height][width] + m[height][width];
        }
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::mutiply(double val)
{
    int height = _height;
    int width;

    //on parcour et on affecte la nouvelle valeur
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = _matrix[height][width] * val;
        }
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::no()
{
    int height = _height;
    int width;

    //on parcour et on affecte la nouvelle valeur
    while(height)
    {
        --height;
        width = _width;
        while(width)
        {
            --width;
            _matrix[height][width] = !_matrix[height][width];
        }
    }
    return *this;
}
#endif // MATRIX_H
