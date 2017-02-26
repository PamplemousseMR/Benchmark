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

        int _height;    /*!< Hauteur de la matrice */
        int _width;     /*!< Largeur de la matrice */
        T** _matrix;    /*!< Les donnees de la matrice */

    public :
        /*!
         * \fn Matrix()
         * \brief Constructeur par default
         */
        Matrix(int, int);
        /*!
         * \fn Matrix(matrix_type,int, int, T=0)
         * \brief Constructeur
         * \param int : Hauteur de la matrice
         * \param int : Largeur de la matrice
         * \param T : Valeur de remplissage
         */
        Matrix(matrix_type,int, int, T=0);
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
         * \fn int getHeight() const
         * \brief Retourn la hauteur de la matrice
         * \return int : La hauteur
         */
        int getHeight() const;
        /*!
         * \fn int getWidth() const
         * \brief Retourn la largeur de la matrice
         * \return int : La largeur
         */
        int getWidth() const;
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
        T* operator[](int) throw(thr);
        #else
        T* operator[](int);
        #endif
        /*!
         * \fn const T* operator[](int) const throw(thr)
         * \brief Surcharge de l'opérateur []
         * \param int : L'index de la valeur a recuperer
         * \return T* : tableau d'entier correspondant à la ligne spécifié en paramètres
         */
        #ifdef DEBUG_EXCEPTION
        const T* operator[](int) const throw(thr);
        #else
        const T* operator[](int) const;
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
            int k=0;
            for(int i=0; i<m.getHeight(); ++i)
            {
                for(int j=0; j<m.getWidth(); ++j)
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
Matrix<T>::Matrix(int height, int width)
    : _height(height),
      _width(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix()] ..." << endl;
    #endif
    _matrix = new T*[_height];
    for(int i=0; i<_height; ++i)
        _matrix[i] = new T[_width];
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix()] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::Matrix(matrix_type type,int height, int width, T value)
    : _height(height), _width(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(matrix_type,int,int," << typeid(T).name() << ")] ..." << endl;
    #endif

    vector<int> vec;
    if(type == randperm)
    {
        for(int i=0; i<width; ++i)
            vec.push_back(i);
        #ifdef RANDOM
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
        #else
        auto engine = default_random_engine();
        #endif
		shuffle(begin(vec), end(vec), engine);
    }
    _matrix = new T*[_height];
    for(int i=0; i<_height; ++i)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; ++j)
        {
            switch(type)
            {
                case standard:
                    _matrix[i][j] = value;
                break;
				case stdrand:
                    _matrix[i][j] = (T)((double)rand() / (RAND_MAX));
                break;
                case randperm:
                    _matrix[i][j] = (T)vec[j];
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

    _matrix = new T*[_height];
    for(int i=0; i<_height; ++i)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = matrix._matrix[i][j];
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

    for(int i=0; i<_height; ++i)
        delete[] _matrix[i];
    delete[] _matrix;

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][~Matrix] Done" << endl;
    #endif
}

template<typename T>
void Matrix<T>::generate(matrix_type type, T value)
{
    vector<int> vec;
    if(type == randperm)
    {
        for(int i=0; i<_width; ++i)
            vec.push_back(i);
        #ifdef RANDOM
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
        #else
        auto engine = default_random_engine();
        #endif
        shuffle(begin(vec), end(vec), engine);
    }
    for(int i=0; i<_height; ++i)
    {
        for(int j=0; j<_width; ++j)
        {
            switch(type)
            {
                case standard:
                    _matrix[i][j] = value;
                break;
                case stdrand:
                    _matrix[i][j] = (T)((double)rand() / (RAND_MAX));
                break;
                case randperm:
                    _matrix[i][j] = (T)vec[j];
                break;
            }
        }
    }
}

template<typename T>
int Matrix<T>::getHeight() const
{
    return _height;
}

template<typename T>
int Matrix<T>::getWidth() const
{
    return _width;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
    if(_height!=m.getHeight() || _width!=m.getWidth())
    {
        for(int i=0; i<_height; ++i)
            delete[] _matrix[i];
        delete[] _matrix;

        _height = m.getHeight();
        _width = m.getWidth();

        _matrix = new T*[_height];
        for(int i=0; i<_height; ++i)
        {
            _matrix[i] = new T[_width];
            for(int j=0; j<_width; ++j)
                _matrix[i][j] = m[i][j];
        }
    }
    else
    {
        for(int i=0; i<_height; ++i)
            for(int j=0; j<_width; ++j)
                _matrix[i][j] = m[i][j];
    }
    return *this;
}


template<typename T>
#ifdef DEBUG_EXCEPTION
T* Matrix<T>::operator[](int index) throw(thr)
#else
T* Matrix<T>::operator[](int index)
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
const T* Matrix<T>::operator[](int index) const throw(thr)
#else
const T* Matrix<T>::operator[](int index) const
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
    for(int i=0; i<_height; ++i)
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = _matrix[i][j]>val;
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
    for(int i=0; i<_height; ++i)
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = _matrix[i][j]>m[i][j] ;
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
    for(int i=0; i<_height; ++i)
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = _matrix[i][j] + m[i][j] ;
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::mutiply(double val)
{
    for(int i=0; i<_height; ++i)
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = (T)(_matrix[i][j] * val);
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::no()
{
    for(int i=0; i<_height; ++i)
        for(int j=0; j<_width; ++j)
            _matrix[i][j] = !_matrix[i][j];
    return *this;
}
#endif // MATRIX_H
