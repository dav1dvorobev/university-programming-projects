#include "rational.h"

rational::rational() : _numerator(0), _denominator(1) {}
rational::rational(const BigInteger& lhs, const BigInteger& rhs) : _numerator(lhs), _denominator(rhs) { (*this).simplify(); }
rational::rational(const rational& other) {
    _numerator = other.numerator();
    _denominator = other.denominator();
    (*this).simplify();
}
rational::rational(rational&& other) noexcept {
    std::swap(_numerator, other._numerator);
    std::swap(_denominator, other._denominator);
}

rational& rational::operator=(const rational& other) {
    if (this != &other) {
        _numerator = other.numerator();
        _denominator = other.denominator();
    }
    (*this).simplify();
    return *this;
}
rational& rational::operator=(rational&& other) noexcept {
    if (this != &other) {
        std::swap(_numerator, other._numerator);
        std::swap(_denominator, other._denominator);
    }
    (*this).simplify();
    return *this;
}

rational& rational::operator+=(const rational& other) {
    if (_denominator == other.denominator()) {
        _numerator += other.numerator();
    } else {
        _numerator = (_numerator * other.denominator()) + (_denominator * other.numerator());
        _denominator *= other.denominator();
    }
    (*this).simplify();
    return *this;
}
rational& rational::operator-=(const rational& other) {
    *this += rational(-other.numerator(), other.denominator());
    return *this;
}
rational& rational::operator*=(const rational& other) {
    _numerator *= other.numerator();
    _denominator *= other.denominator();
    (*this).simplify();
    return *this;
}
rational& rational::operator/=(const rational& other) {
    _numerator *= other.denominator();
    _denominator *= other.numerator();
    (*this).simplify();
    return *this;
}

bool rational::operator>(const rational& other) {
    if (_denominator == other.denominator()) { return _numerator > other.numerator(); }
    return (_numerator * other.denominator()) > (other.numerator() * _denominator);
}

rational rational::operator+(const rational& other) {
    rational output = *this;
    output += other;
    return output;
}
rational rational::operator-(const rational& other) { return *this + rational(-other.numerator(), other.denominator()); }
rational rational::operator*(const rational& other) {
    rational output = *this;
    output *= other;
    return output;
}
rational rational::operator/(const rational& other) {
    rational output = *this;
    output /= other;
    return output;
}

rational abs(const rational& other) { return (!other.numerator().negative()) ? other : rational(-other.numerator(), other.denominator()); }
std::string to_string(const rational& other) {
    std::string output;
    output = to_string(other.numerator());
    if (other.denominator() != 1) { output += "/" + to_string(other.denominator()); }
    return output;
}
std::ostream& operator<<(std::ostream& output, const rational& other) {
    output << to_string(other);
    return output;
}