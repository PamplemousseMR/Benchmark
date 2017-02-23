#include "matrix.h"

using namespace std;

Matrix::Matrix()
{
    #ifdef DEBUG
    cout << "[Matrix][Matrix] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Matrix][Matrix] Done" << endl;
    #endif
}

Matrix::Matrix(const Matrix&)
{
    #ifdef DEBUG
    cout << "[Matrix][Matrix(const Matrix&)] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Matrix][Matrix(const Matrix&)] Done" << endl;
    #endif
}

Matrix::~Matrix()
{
    #ifdef DEBUG
    cout << "[Matrix][~Matrix] ..." << endl;
    #endif
    #ifdef DEBUG
    cout << "[Matrix][~Matrix] Done" << endl;
    #endif
}

ostream& operator<<(ostream& flux, const Matrix& m)
{
    return flux;
}
