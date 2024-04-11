#ifndef BIGINTEGER
#define BIGINTEGER

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigInteger {
    bool _negative;
public:
    std::vector<unsigned long long> arr;

    BigInteger();
    BigInteger(long long);
    BigInteger(std::string);
    BigInteger(const BigInteger&);
    BigInteger(BigInteger&&) noexcept;

    BigInteger& operator=(const BigInteger&);
    BigInteger& operator=(BigInteger&&) noexcept;

    BigInteger& operator+=(const BigInteger&);
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator*=(const BigInteger&);
    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);

    bool operator==(const BigInteger&) const;
    bool operator!=(const BigInteger&) const;
    bool operator>(const BigInteger&) const;
    bool operator<(const BigInteger&) const;
    bool operator<=(const BigInteger&) const;

    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;

    BigInteger operator-() const;

    bool negative() const { return _negative; }
    size_t size() const { return arr.size(); }
};
BigInteger abs(const BigInteger&);
std::string to_string(const BigInteger&);
std::ostream& operator<<(std::ostream&, const BigInteger&);

#endif // BIGINTEGER