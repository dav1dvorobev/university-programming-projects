#include "matrix.h"

void triangular(matrix& other, bool inv_flag = false) {
    for (int i = 0; i < other.rows() - 1; ++i) {
        int maxx = i;
        if (!inv_flag) {
            for (int j = i + 1; j < other.rows(); ++j) {
                if (abs(other.arr[j][i]) > abs(other.arr[maxx][i])) { maxx = j; }
            }
            if (maxx != i) {
                other.arr[i].swap(other.arr[maxx]);
                for (int j = 0; j < other.cols(); ++j) { other.arr[i][j] *= rational(-1, 1); }
            }
        }
        if (to_string(other.arr[i][i]) != "0") {
            for (int j = i + 1; j < other.rows(); ++j) {
                rational factor = other.arr[j][i] / other.arr[i][i];
                for (int k = i; k < other.cols(); ++k) { other.arr[j][k] -= factor * other.arr[i][k]; }
            }
        }
    }
}

matrix::matrix() {
    *this = matrix(1, 1);
    arr[0][0] = rational(0, 1);
}
matrix::matrix(int other) {
    *this = matrix(1, 1);
    arr[0][0] = rational(other, 1);
}
matrix::matrix(const rational& other) {
    *this = matrix(1, 1);
    arr[0][0] = other;
}
matrix::matrix(int lhs, int rhs) : _rows(lhs), _cols(rhs) { arr.resize(_rows, std::vector<rational>(_cols)); }
matrix::matrix(const matrix& other) {
    _rows = other.rows();
    _cols = other.cols();
    arr = other.arr;
}
matrix::matrix(matrix&& other) noexcept {
    std::swap(_rows, other._rows);
    std::swap(_cols, other._cols);
    std::swap(arr, other.arr);
}

matrix& matrix::operator=(const matrix& other) {
    if (this != &other) {
        matrix copy = other;
        std::swap(_rows, copy._rows);
        std::swap(_cols, copy._cols);
        std::swap(arr, copy.arr);
    }
    return *this;
}
matrix& matrix::operator=(matrix&& other) noexcept {
    if (this != &other) {
        std::swap(_rows, other._rows);
        std::swap(_cols, other._cols);
        std::swap(arr, other.arr);
    }
    return *this;
}

rational matrix::det() const {
    rational determinant(1, 1);
    matrix temp = *this;
    triangular(temp);
    for (int i = 0; i < temp.rows(); ++i) { determinant *= temp.arr[i][i]; }
    return determinant;
}
rational matrix::sum() const {
    rational output;
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) { output += arr[i][j]; }
    }
    return output;
}
rational matrix::prod() const {
    rational output(1, 1);
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) { output *= arr[i][j]; }
    }
    return output;
}

matrix& matrix::operator+=(const matrix& other) {
    for (int i = 0; i < other.rows(); ++i) {
        for (int j = 0; j < other.cols(); ++j) { arr[i][j] += other.arr[i][j]; }
    }
    return *this;
}
matrix& matrix::operator-=(const matrix& other) {
    for (int i = 0; i < other.rows(); ++i) {
        for (int j = 0; j < other.rows(); ++j) { arr[i][j] -= other.arr[i][j]; }
    }
    return *this;
}
matrix& matrix::operator*=(const matrix& other) {
    matrix copy = *this;
    *this = zeros(copy.rows(), other.cols());
    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            for (int k = 0; k < copy.cols(); ++k) { arr[i][j] += copy.arr[i][k] * other.arr[k][j]; }
        }
    }
    return *this;
}
matrix& matrix::operator/=(const matrix& other) {
    *this *= inv(other);
    return *this;
}

matrix matrix::operator+(const matrix& other) const {
    matrix copy = *this;
    copy += other;
    return copy;
}
matrix matrix::operator-(const matrix& other) const {
    matrix copy = *this;
    copy -= other;
    return copy;
}
matrix matrix::operator*(const matrix& other) const {
    matrix copy = *this;
    copy *= other;
    return copy;
}
matrix matrix::operator/(const matrix& other) const {
    matrix copy = *this;
    copy /= other;
    return copy;
}

matrix matrix::operator-() const {
    matrix copy = *this;
    for (long long i = 0; i < copy.rows(); ++i) {
        for (long long j = 0; j < copy.cols(); ++j) { copy.arr[i][j] *= rational(-1, 1); }
    }
    return copy;
}

matrix prod(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < output.rows(); ++i) {
        for (int j = 0; j < output.cols(); ++j) { output.arr[i][j] *= rhs.arr[i][j]; }
    }
    return output;
}
matrix div(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < lhs.rows(); ++i) {
        for (int j = 0; j < rhs.rows(); ++j) { output.arr[i][j] /= rhs.arr[i][j]; }
    }
    return output;
}
matrix upheaval(const matrix& other) {
    matrix output(other.rows(), other.cols());
    for (int i = 0; i < other.rows(); ++i) {
        for (int j = 0; j < other.cols(); ++j) { output.arr[i][j] = other.arr[other.rows() - i - 1][other.cols() - j - 1]; }
    }
    return output;
}
matrix zeros(const int& rows, const int& cols) {
    matrix output(rows, cols);
    return output;
}
matrix ones(const int& rows, const int& cols) {
    matrix output(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) { output.arr[i][j] = rational(1, 1); }
    }
    return output;
}
matrix eye(const int& rows, const int& cols) {
    matrix output(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == j) { output.arr[i][j] = rational(1, 1); }
        }
    }
    return output;
}
matrix transpose(const matrix& other) {
    matrix output = matrix(other.cols(), other.rows());
    for (int i = 0; i < output.rows(); ++i) {
        for (int j = 0; j < output.cols(); ++j) { output.arr[i][j] = other.arr[j][i]; }
    }
    return output;
}
matrix inv(const matrix& other) {
    matrix lhs = horzcat(other, eye(other.rows(), other.cols()));
    triangular(lhs);
    matrix rhs(other.rows(), other.cols());
    for (int i = 0; i < rhs.rows(); ++i) {
        for (int j = 0; j < rhs.cols(); ++j) { rhs.arr[i][j] = lhs.arr[i][j + rhs.cols()]; }
    }
    lhs.resize(other.rows(), other.cols());
    lhs = horzcat(upheaval(lhs), upheaval(rhs));
    triangular(lhs, true);
    for (int i = 0; i < lhs.rows(); ++i) {
        rational temp = lhs.arr[i][i];
        for (int j = i; j < lhs.cols(); ++j) { lhs.arr[i][j] /= temp; }
    }
    for (int i = 0; i < rhs.rows(); ++i) {
        for (int j = 0; j < rhs.cols(); ++j) { rhs.arr[i][j] = lhs.arr[i][j + rhs.cols()]; }
    }
    return upheaval(rhs);
}
matrix linspace(const int& lhs, const int& rhs, const int& size) {
    matrix output(1, size);
    for (int i = 0, x = lhs; i < size; ++i, ++x) { output.arr[0][i] = rational(x, 1); }
    return output;
}
matrix vertcat(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < rhs.rows(); ++i) { output.arr.push_back(rhs.arr[i]); }
    output.resize(lhs.rows() + rhs.rows(), output.cols());
    return output;
}
matrix horzcat(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < lhs.rows(); ++i) {
        for (int j = 0; j < rhs.cols(); ++j) { output.arr[i].push_back(rhs.arr[i][j]); }
    }
    output.resize(output.rows(), lhs.cols() + rhs.cols());
    return output;
}
matrix min(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < output.rows(); ++i) {
        for (int j = 0; j < output.cols(); ++j) {
            if (output.arr[i][j] > rhs.arr[i][j]) { output.arr[i][j] = rhs.arr[i][j]; }
        }
    }
    return output;
}
matrix max(const matrix& lhs, const matrix& rhs) {
    matrix output = lhs;
    for (int i = 0; i < output.rows(); ++i) {
        for (int j = 0; j < output.cols(); ++j) {
            if (!(output.arr[i][j] > rhs.arr[i][j])) { output.arr[i][j] = rhs.arr[i][j]; }
        }
    }
    return output;
}

int format(const matrix& other, int j) {
    int maxx = 0;
    for (int i = 0; i < other.rows(); ++i) { maxx = std::max(maxx, static_cast<int>(to_string(other.arr[i][j]).size())); }
    return maxx;
}
std::ostream& operator<<(std::ostream& output, const matrix& other) {
    for (int i = 0; i < other.rows(); ++i) {
        for (int j = 0; j < other.cols(); ++j) {
            (j == 0) ? output << std::setw(format(other, j)) : output << std::setw(format(other, j) + 1);
            output << to_string(other.arr[i][j]);
        }
        if (i != other.rows() - 1) { output << '\n'; }
    }
    return output;
}
matrix operator*(const matrix& other, const rational& num) {
    matrix copy = other;
    for (int i = 0; i < other.rows(); ++i) {
        for (int j = 0; j < other.cols(); ++j) { copy.arr[i][j] *= num; }
    }
    return copy;
}