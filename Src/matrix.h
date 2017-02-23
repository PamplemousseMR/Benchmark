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
    public :
        /*!
         * \brief Matrix constructeur
         */
        Matrix();
        /*!
         * \brief Matrix constructeur par copie
         */
        Matrix(const Matrix&);
        /*!
         * \brief ~Matrix destructeur
         */
        ~Matrix();
        /*!
         * \brief operator << permet l'affichage d'une matrice
         * \return le stream passe en parametre modifie
         */
        friend std::ostream& operator<<(std::ostream&, const Matrix&);
};

#endif // MATRIX_H
