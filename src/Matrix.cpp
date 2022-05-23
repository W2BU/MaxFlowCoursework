#pragma once
#include <iostream>
#include <stdexcept>

class Matrix
{
    public:
        Matrix(int, int);
        Matrix(double**, int, int);
        Matrix();
        ~Matrix();
        Matrix(const Matrix&);
        Matrix& operator=(const Matrix&);

        inline double& operator()(int x, int y) { return matrix[x][y]; }

        static Matrix createIdentity(int);

        Matrix& operator+=(const Matrix&);
        Matrix& operator-=(const Matrix&);
        Matrix& operator*=(const Matrix&);
        Matrix& operator*=(double);
        Matrix& operator/=(double);
        Matrix  operator^(int);
        
        friend std::ostream& operator<<(std::ostream&, const Matrix&);
        friend std::istream& operator>>(std::istream&, Matrix&);

    private:
        int rows_, cols_;
        double **matrix;

        void allocSpace();
        Matrix expHelper(const Matrix&, int);
};

Matrix operator+(const Matrix&, const Matrix&);
Matrix operator-(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, double);
Matrix operator*(double, const Matrix&);
Matrix operator/(const Matrix&, double);


using std::ostream;  using std::istream;  using std::endl;
using std::domain_error;

/* PUBLIC MEMBER FUNCTIONS
 ********************************/

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = 0;
        }
    }
}

Matrix::Matrix(double** otherMatrix, int rows, int cols) : rows_(rows), cols_(cols)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = otherMatrix[i][j];
        }
    }
}

Matrix::Matrix() : rows_(1), cols_(1)
{
    allocSpace();
    matrix[0][0] = 0;
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows_; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_)
{
    allocSpace();
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = other.matrix[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other) {
        return *this;
    }

    if (rows_ != other.rows_ || cols_ != other.cols_) {
        for (int i = 0; i < rows_; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;

        rows_ = other.rows_;
        cols_ = other.cols_;
        allocSpace();
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] = other.matrix[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] += other.matrix[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other)
{
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] -= other.matrix[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    Matrix temp(rows_, other.cols_);
    for (int i = 0; i < temp.rows_; ++i) {
        for (int j = 0; j < temp.cols_; ++j) {
            for (int k = 0; k < cols_; ++k) {
                temp.matrix[i][j] += (matrix[i][k] * other.matrix[k][j]);
            }
        }
    }
    return (*this = temp);
}

Matrix& Matrix::operator*=(double number)
{
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] *= number;
        }
    }
    return *this;
}

Matrix& Matrix::operator/=(double number)
{
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            matrix[i][j] /= number;
        }
    }
    return *this;
}

Matrix Matrix::operator^(int number)
{
    Matrix temp(*this);
    return expHelper(temp, number);
}

Matrix Matrix::createIdentity(int size)
{
    Matrix temp(size, size);
    for (int i = 0; i < temp.rows_; ++i) {
        for (int j = 0; j < temp.cols_; ++j) {
            if (i == j) {
                temp.matrix[i][j] = 1;
            } else {
                temp.matrix[i][j] = 0;
            }
        }
    }
    return temp;
}

/* PRIVATE HELPER FUNCTIONS
 ********************************/

void Matrix::allocSpace()
{
    matrix = new double*[rows_];
    for (int i = 0; i < rows_; ++i) {
        matrix[i] = new double[cols_];
    }
}

Matrix Matrix::expHelper(const Matrix& other, int number)
{
    if (number == 0) { 
        return createIdentity(other.rows_);
    } else if (number == 1) {
        return other;
    } else if (number % 2 == 0) {  // number is even
        return expHelper(other * other, number/2);
    } else {                    // number is odd
        return other * expHelper(other * other, (number-1)/2);
    }
}

/* NON-MEMBER FUNCTIONS
 ********************************/

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
    Matrix temp(m1);
    return (temp += m2);
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
    Matrix temp(m1);
    return (temp -= m2);
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    Matrix temp(m1);
    return (temp *= m2);
}

Matrix operator*(const Matrix& other, double number)
{
    Matrix temp(other);
    return (temp *= number);
}

Matrix operator*(double number, const Matrix& other)
{
    return (other * number);
}

Matrix operator/(const Matrix& other, double number)
{
    Matrix temp(other);
    return (temp /= number);
}

ostream& operator<<(ostream& os, const Matrix& other)
{
    for (int i = 0; i < other.rows_; ++i) {
        os << other.matrix[i][0];
        for (int j = 1; j < other.cols_; ++j) {
            os << " " << other.matrix[i][j];
        }
        os << endl;
    }
    return os;
}

istream& operator>>(istream& is, Matrix& other)
{
    for (int i = 0; i < other.rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            is >> other.matrix[i][j];
        }
    }
    return is;
}