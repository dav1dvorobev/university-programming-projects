#ifndef MATRIX
#define MATRIX

#include <iomanip>
#include <iostream>
#include <vector>

#include "rational.h"

class matrix {
    int _rows;
    int _cols;
public:
    std::vector<std::vector<rational>> arr;
    matrix();
    matrix(int);
    matrix(const rational&);
    matrix(int, int);
    matrix(const matrix&);
    matrix(matrix&&) noexcept;

    matrix& operator=(const matrix&);
    matrix& operator=(matrix&&) noexcept;

    matrix& operator+=(const matrix&);
    matrix& operator-=(const matrix&);
    matrix& operator*=(const matrix&);
    matrix& operator/=(const matrix&);

    matrix operator+(const matrix&) const;
    matrix operator-(const matrix&) const;
    matrix operator*(const matrix&) const;
    matrix operator/(const matrix&) const;

    matrix operator-() const;

    rational det() const;
    rational sum() const;
    rational prod() const;

    matrix diag() const {
        matrix diag(1, _cols);
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                if (i == j) { diag.arr[0][j] = arr[i][j]; }
            }
        }
        return diag;
    }

    const int& rows() const { return _rows; }
    const int& cols() const { return _cols; }
    void resize(const int& lhs, const int& rhs) {
        _rows = lhs;
        _cols = rhs;
    }

    operator int() const { return stoi(to_string(arr[0][0].numerator())); }
    operator rational() const { return arr[0][0]; }
};
std::ostream& operator<<(std::ostream&, const matrix&);
matrix operator*(const matrix&, const rational&);

matrix prod(const matrix&, const matrix&);
matrix div(const matrix&, const matrix&);
matrix zeros(const int&, const int&);
matrix ones(const int&, const int&);
matrix eye(const int&, const int&);
matrix linspace(const int&, const int&, const int&);
matrix vertcat(const matrix&, const matrix&);
matrix horzcat(const matrix&, const matrix&);
matrix transpose(const matrix&);
matrix inv(const matrix&);
matrix min(const matrix&, const matrix&);
matrix max(const matrix&, const matrix&);

#endif // MATRIX