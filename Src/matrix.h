#ifndef MATRIX_H
#define MATRIX_H
/*!
 * \file Matrix.h
 * \brief contient la classe representant une matrice
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
#include <iostream> //cout
#include <stdlib.h> //rand
#include <stdexcept> //length_error
#include <vector> //vector
#include <algorithm> //random_shuffle
#include <random> //default_random_engine
#include <chrono> // chrono::system_clock
/*!
 * \enum matrix_type le type de construction de la matrice
 */
enum matrix_type
{
    standard,
    random,
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
        /*!
         * \brief _height hauteur de la matrice
         */
        int _height;
        /*!
         * \brief _width largeur de la matrice
         */
        int _width;
        /*!
         * \brief _matrix les donnees de la matrice
         */
        T** _matrix;
    public :
        /*!
         * \brief Matrix Constructeur
         * \param int Hauteur de la matrice
         * \param int Largeur de la matrice
         * \param T Valeur de remplissage
         */
        Matrix(matrix_type,int, int, T=0);
        /*!
         * \brief Matrix constructeur par copie
         */
        Matrix(const Matrix&);
        /*!
         * \brief ~Matrix destructeur
         */
        ~Matrix();
        /*!
         * \brief getHeight retourn la hauteur de la matrice
         * \return int la hauteur
         */
        int getHeight() const;
        /*!
         * \brief getWidht retourn la largeur de la matrice
         * \return int la largeur
         */
        int getWidht() const;
        /*!
         * \brief superior modifie la matrice en fonction d'une autre matrice avec l'operateur >
         * \param double la valeur a verifier
         * \return la matrice courente modifie
         */
        Matrix& superior(double);
        /*!
         * \brief superior modifie la matrice en fonction d'une autre matrice avec l'operateur >
         * \param const Matrix<E>& la matrice a comparer
         * \return la matrice modifie
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
        Matrix& superior(const Matrix<E>&) throw(...);
        /*!
         * \brief superior modifie la matrice en fonction d'une autre matrice avec l'operateur +
         * \param const Matrix<E>& la matrice a additionner
         * \return la matrice modifie
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
        Matrix& add(const Matrix<E>&) throw(...);
        /*!
         * \brief superior multiplie la matrice par la valeur
         * \param double la valeur a multiplier
         * \return la matrice modifie
         */
        Matrix& mutiply(double);
        /*!
         * \brief operator = Surcharge de l'opérateur =
         * \return matrix
         */
        Matrix& operator=(const Matrix&);
        /*!
         * \brief operator [] Surcharge de l'opérateur []
         * \return tableau d'entier correspondant à la ligne spécifié en paramètres
         */
        T* operator[](int);
        /*!
         * \brief operator [] Surcharge de l'opérateur []
         * \return tableau d'entier correspondant à la ligne spécifié en paramètres
         */
        const T* operator[](int) const;
        /*!
         * \brief operator* surcharge de l'operateur*
         * \param double la valeur a multiplier
         * \return la matrice multiplier
         */
        Matrix operator*(double) const;
        /*!
         * \brief operator+ surcharge de l'operateur+
         * \param const Matrix<E>& la matrice a additionner
         * \return la matrice additionner
         * \throws length_error si la taille des matrices est differente
         */
        template<typename E>
        Matrix operator+(const Matrix<E>&) const throw(...);
        /*!
         * \brief operator+ surcharge de l'operateur+
         * \param double la valeur a additionner
         * \return la matrice additionner
         */
        Matrix operator+(double) const;
        /*!
         * \brief operator ! la matrice oposer (operateur! sur le T)
         * \return la matrice opose
         */
        Matrix operator!() const;
        /*!
         * \brief operator << permet l'affichage d'une matrice
         * \param std::ostream& le flux
         * \param const Matrix& la matrice a afficher
         * \return le stream passe en parametre modifie
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
         * \brief operator* permet de multiplier une matrice par une valeur
         * \param double la valeur
         * \param const Matrix& la matrice a multiplier
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
         * \brief operator* permet d'additionner une matrice par une valeur
         * \param double la valeur
         * \param const Matrix& la matrice a additionner
         * \return Matrix<T> la matrice additionner
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
    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(int,int,T)] ..." << endl;
    #endif
    vector<int> vec(width);
    if(type == randperm)
    {
        for(int i=0; i<width; i++)
            vec[i] = i;
        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        auto engine = default_random_engine(seed);
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
                case random:
                    _matrix[i][j] = (T)((double)rand() / (RAND_MAX));
                break;
                case randperm:
                    _matrix[i][j] = vec[j];
                break;
            }
        }
    }
    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(int,int,T)] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& matrix)
    : _height(matrix._height), _width(matrix._width)
{
    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(const Matrix&)] ..." << endl;
    #endif

    _matrix = new T*[_height];
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new T[_width];
        for(int j=0; j<_width; j++)
            _matrix[i][j] = matrix._matrix[i][j];
    }

    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][Matrix(const Matrix&)] Done" << endl;
    #endif
}

template<typename T>
Matrix<T>::~Matrix()
{
    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][~Matrix] ..." << endl;
    #endif

    for(int i=0; i<_height; i++)
    {
        delete[] _matrix[i];
    }
    delete[] _matrix;

    #ifdef DEBUG_0
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
    #ifdef DEBUG_0
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
    #ifdef DEBUG_0
    cout << "[Matrix<" << typeid(T).name() << ">][operator=] Done" << endl;
    #endif
    return *this;
}
template<typename T>
T* Matrix<T>::operator[](int index)
{
    if(index < 0)
        return _matrix[0];
    else if(index >= _height)
        return _matrix[_height-1];
    else
        return _matrix[index];
}

template<typename T>
const T* Matrix<T>::operator[](int index) const
{
    if(index < 0)
        return _matrix[0];
    else if(index >= _height)
        return _matrix[_height-1];
    else
        return _matrix[index];
}

template<typename T>
Matrix<T>& Matrix<T>::superior(double val)
{
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = _matrix[i][j] > val;
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
Matrix<T>& Matrix<T>::superior(const Matrix<E>& m) throw(...)
{
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
    for(int i=0; i<_height; i++)
        for(int j=0; j<_width; j++)
            _matrix[i][j] = _matrix[i][j]>m[i][j] ;
    return *this;
}

template<typename T>
template<typename E>
Matrix<T>& Matrix<T>::add(const Matrix<E>& m) throw(...)
{
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
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
template<typename E>
Matrix<T> Matrix<T>::operator+(const Matrix<E>& m) const throw(...)
{
    if(_height!=m.getHeight() || _width!=m.getWidht())
        throw length_error("matrix length are not equals");
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
