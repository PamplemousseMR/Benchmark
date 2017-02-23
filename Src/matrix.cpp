#include "matrix.h"

using namespace std;

Matrix::Matrix(int height, int width) : _height(height), _width(width)
{
    #ifdef DEBUG
    cout << "[Matrix][Matrix] ..." << endl;
    #endif
    _matrix = new int*[_height];
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new int[_width];
        for(int j=0; j<_width; j++)
        {
            _matrix[i][j] = 0;
        }
    }
    #ifdef DEBUG
    cout << "[Matrix][Matrix] Done" << endl;
    #endif
}

Matrix::Matrix(const Matrix& matrix) : _height(matrix._height), _width(matrix._width)
{
    #ifdef DEBUG
    cout << "[Matrix][Matrix(const Matrix&)] ..." << endl;
    #endif

    _matrix = new int*[_height];
    for(int i=0; i<_height; i++)
    {
        _matrix[i] = new int[_width];
        for(int j=0; j<_width; j++)
        {
            _matrix[i][j] = matrix._matrix[_height][_width];
        }
    }

    #ifdef DEBUG
    cout << "[Matrix][Matrix(const Matrix&)] Done" << endl;
    #endif
}

Matrix::~Matrix()
{
    #ifdef DEBUG
    cout << "[Matrix][~Matrix] ..." << endl;
    #endif

    for(int i=0; i<_height; i++)
    {
        delete[] _matrix[i];
    }
    delete[] _matrix;

    #ifdef DEBUG
    cout << "[Matrix][~Matrix] Done" << endl;
    #endif
}

ostream& operator<<(ostream& flux, const Matrix& m)
{
    for(int i=0; i<m._height; i++) {
        for(int j=0; j<m._width; j++) {
            flux << m[i][j];
        }
        flux << "\n";
    }
    return flux;
}
