#ifndef BIGINTEGER
#define BIGINTEGER

#include <iostream>

class BigInteger {
    size_t _size;
    bool _negative;
    unsigned long long* _arr;
    void push_back(const unsigned long long& input) {
        unsigned long long* tmp = new unsigned long long[_size + 1];
        for (size_t i = 0; i < _size; ++i) { tmp[i] = _arr[i]; }
        tmp[_size] = input;
        std::swap(tmp, _arr);
        ++_size;
        delete[] tmp;
    }
    void pop_back() {
        unsigned long long* tmp = new unsigned long long[_size - 1];
        for (size_t i = 0; i < _size - 1; ++i) { tmp[i] = _arr[i]; }
        std::swap(tmp, _arr);
        --_size;
        delete[] tmp;
    }
    void resize(const size_t& size) {
        delete[] _arr;
        _size = size;
        _arr = new unsigned long long[_size];
        for (size_t i = 0; i < _size; ++i) { _arr[i] = 0; }
    }
    void swap(BigInteger& other) {
        std::swap(_size, other._size);
        std::swap(_negative, other._negative);
        std::swap(_arr, other._arr);
    }
public:
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
    bool operator>=(const BigInteger&) const;

    BigInteger operator+(const BigInteger&) const;
    BigInteger operator-(const BigInteger&) const;
    BigInteger operator*(const BigInteger&) const;
    BigInteger operator/(const BigInteger&) const;
    BigInteger operator%(const BigInteger&) const;

    BigInteger operator+() const;
    BigInteger operator-() const;

    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);

    size_t size() const { return _size; }
    bool negative() const { return _negative; }
    unsigned long long operator[](const size_t& idx) const {
        if (idx < _size) { return _arr[idx]; }
        throw std::out_of_range("Index is out of range");
    }
    ~BigInteger() {
        delete[] _arr;
        _arr = nullptr;
    }
};
BigInteger operator+(const int&, const BigInteger&);
bool operator==(const int&, const BigInteger&);
BigInteger abs(const BigInteger&);
std::string to_string(const BigInteger&);
std::ostream& operator<<(std::ostream&, const BigInteger&);

#endif // BIGINTEGER