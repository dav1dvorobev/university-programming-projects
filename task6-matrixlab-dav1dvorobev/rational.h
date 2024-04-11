#ifndef RATIONAL
#define RATIONAL

#include <iostream>

#include "biginteger.h"

class rational {
    BigInteger _numerator;
    BigInteger _denominator;
    BigInteger static gcd(BigInteger lhs, BigInteger rhs) {
        while (rhs != 0) {
            BigInteger copy = rhs;
            rhs = lhs % rhs;
            lhs = copy;
        }
        return lhs;
    }
    void simplify() {
        BigInteger tmp = gcd(_numerator, _denominator);
        if (tmp != 0) {
            _numerator /= tmp;
            _denominator /= tmp;
        }
        if (_denominator.negative()) {
            _numerator *= -1;
            _denominator *= -1;
        }
    }
public:
    rational();
    rational(const BigInteger&, const BigInteger&);
    rational(const rational&);
    rational(rational&&) noexcept;

    rational& operator=(const rational&);
    rational& operator=(rational&&) noexcept;

    rational& operator+=(const rational&);
    rational& operator-=(const rational&);
    rational& operator*=(const rational&);
    rational& operator/=(const rational&);

    bool operator>(const rational&);

    rational operator+(const rational&);
    rational operator-(const rational&);
    rational operator*(const rational&);
    rational operator/(const rational&);

    const BigInteger& numerator() const { return _numerator; }
    const BigInteger& denominator() const { return _denominator; }
};
rational abs(const rational&);
std::string to_string(const rational&);
std::ostream& operator<<(std::ostream&, const rational&);

#endif // RATIONAL