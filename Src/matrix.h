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

        int _height;/*!< Hauteur de la matrice */
        int _width;/*!< Largeur de la matrice */
        T** _matrix;/*!< Les donnees de la matrice */

    public :
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
         * \fn int getHeight() const
         * \brief Retourn la hauteur de la matrice
         * \return int : La hauteur
         */
        int getHeight() const;
        /*!
         * \fn int getWidht() const
         * \brief Retourn la largeur de la matrice
         * \return int : La largeur
         */
        int getWidht() const;
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
         * \fn Matrix& soustract(double)
         * \brief Soustrait la valeur a chaque element de la matrice
         * \param double : la valeur a soustraire
         * \return Matrix<T> La matrice modifie
         */
        Matrix& soustract(double);
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
        Matrix& operator=(const Matrix&);
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
         * \fn Matrix operator*(double) const
         * \brief Surcharge de l'operateur*
         * \param double : la valeur a multiplier
         * \return Matrix<T> Une matrice multiplier
         */
        Matrix operator*(double) const;
        /*!
         * \fn Matrix operator+(const Matrix<E>&) const throw(thr)
         * \brief Surcharge de l'operateur+
         * \param Matrix<T> La matrice a additionner
         * \return Matrix<T> La matrice additionner
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
#ifdef DEBUG_EXCEPTION
        Matrix operator+(const Matrix<E>&) const throw(thr);
#else
        Matrix operator+(const Matrix<E>&) const;
#endif
        /*!
         * \fn Matrix operator+(double) const
         * \brief Surcharge de l'operateur+
         * \param double : La valeur a additionner
         * \return Matrix<T> La matrice additionner
         */
        Matrix operator+(double) const;
        /*!
         * \fn Matrix operator!() const
         * \brief Calcule la matrice oposer (operateur! sur le T)
         * \return Matrix<T>  La matrice opose
         */
        Matrix operator!() const;
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
            for(int i=0; i<m._height; i++)
            {
                for(int j=0; j<m._width; j++)
                {
                    flux << m._matrix[i][j];
                    if(j < m._width-1)
                        flux << ", ";
                }
                if(k < m._height-1)
                    flux << "\n";
                k++;
            }
            return flux;
        }
        /*!
         * \fn friend Matrix<T> operator*(double val, const Matrix<T>& mat)
         * \brief Permet de multiplier une matrice par une valeur
         * \param double : La valeur
         * \param Matrix<T> La matrice a multiplier
         * \return Matrix<T> la matrice multiplier
         */
        friend Matrix<T> operator*(double val, const Matrix<T>& mat)
        {
            Matrix<T> m(standard,mat.getHeight(),mat.getWidht());
            for(int i=0; i<mat.getHeight(); i++)
                for(int j=0; j<mat.getWidht(); j++)
                    m[i][j] = mat[i][j] * val;
            return m;
        }
        /*!
         * \fn friend Matrix<T> operator+(double val, const Matrix<T>& mat)
         * \brief Permet d'additionner une matrice par une valeur
         * \param double : La valeur
         * \param Matrix : La matrice a additionner
         * \return Matrix<T> : La matrice additionner
         */
        friend Matrix<T> operator+(double val, const Matrix<T>& mat)
        {
            Matrix<T> m(mat.getHeight(),mat.getWidht());
            for(int i=0; i<mat.getHeight(); i++)
                for(int j=0; j<mat.getWidht(); j++)
                    m[i][j] = mat[i][j] + val;
            return m;
        }
};

using namespace std;

template<typename T>
Matrix<T>::Matrix(matrix_type type,int height, int width, T value)
    : _height(height), _width(width)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(int,int,T)] ..." << endl;
    #endif
    vector<int> vec(width);
    if(type == randperm)
    {
        for(int i=0; i<width; i++)
            vec[i] = i;
#ifdef RANDOM
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
#else
        auto engine = default_random_engine();
#endif
		shuffle(begin(vec), end(vec), engine);
    }
    _matrix = new T*[_height];
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; j++)
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
                    _matrix[i][j] = vec[j];
                break;
            }
        }
    }
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(int,int,T)] Done" << endl;
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
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; j++)
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

    for(int i=0; i<_height; i++)
        delete[] _matrix[i];
    delete[] _matrix;

    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][~Matrix] Done" << endl;
    #endif
}

template<typename T>
int Matrix<T>::getHeight() const
{
    return _height;
}

template<typename T>
int Matrix<T>::getWidht() const
{
    return _width;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][operator=] ..." << endl;
    #endif

    for(int i=0; i<_height; i++)
        delete[] _matrix[i];
    delete[] _matrix;

    _height = m.getHeight();
    _width = m.getWidht();

    _matrix = new T*[_height];
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; j++)
            _matrix[i][j] = m[i][j];
    }
    #ifdef DEBUG_OUTPUT
    cout << "[Matrix<" << typeid(T).name() << ">][operator=] Done" << endl;
    #endif
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
        throw length_error("index out of bound");
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
        throw length_error("index out of bound");
#endif
        return _matrix[index];
}

template<typename T>
Matrix<T>& Matrix<T>::superior(double val)
{
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = _matrix[i][j]>val;
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(double val) const
{
    Matrix<T> m(_height,_width);
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            m[i][j] = _matrix[i][j] * val;
    return m;
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
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
#endif
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
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
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
#endif
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = _matrix[i][j] + m[i][j] ;
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::mutiply(double val)
{
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = (T)(_matrix[i][j] * val);
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::soustract(double val)
{
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = (T)(_matrix[i][j] - val);
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::no()
{
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = !_matrix[i][j];
    return *this;
}

template<typename T>
template<typename E>
#ifdef DEBUG_EXCEPTION
Matrix<T> Matrix<T>::operator+(const Matrix<E>& m) const throw(thr)
#else
Matrix<T> Matrix<T>::operator+(const Matrix<E>& m) const
#endif
{
#ifdef DEBUG_EXCEPTION
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
#endif
    Matrix<T> mat(standard,_height,_width);
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            mat[i][j] = _matrix[i][j] + m[i][j];
    return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(double val) const
{
    Matrix<T> m(_height,_width);
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            m[i][j] = _matrix[i][j] + val;
    return m;
}

template<typename T>
Matrix<T> Matrix<T>::operator!() const
{
    Matrix<T> m(standard, _height,_width);
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            m[i][j] = !_matrix[i][j];
    return m;
}
#endif // MATRIX_H
