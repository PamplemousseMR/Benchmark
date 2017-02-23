#ifndef MATRIX_H
#define MATRIX_H
/*!
 * \file Matrix.h
 * \brief contient la classe representant une matrice
 * \author Appert Kevin, Bocahu Florent, Hun Tony, Lataix Maxime, Manciaux Romain, Peccard Remi
 */
#include <iostream>
/*!
 * \class Matrix
 * \brief classe representant une matrice
 */
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
        int ** _matrix;
    public :
        /*!
         * \brief Matrix Constructeur par défaut
         * \param height Hauteur de la matrice
         * \param width Largeur de la matrice
         */
        Matrix(int, int);
        /*!
         * \brief Matrix constructeur par copie
         */
        Matrix(const Matrix&);
        /*!
         * \brief ~Matrix destructeur
         */
        ~Matrix();
        Matrix& operator=(const Matrix&);
        int* operator[](int);
        const int* operator[](int) const;
        /*!
         * \brief operator << permet l'affichage d'une matrice
         * \return le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream&, const Matrix&);
};

#endif // MATRIX_H
