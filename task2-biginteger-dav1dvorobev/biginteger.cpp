#include "biginteger.h"

const long long base = 4294967296;

void errors(const std::string& input) {
    if (input.empty()) { throw std::invalid_argument("Invalid syntax"); }
    if (input.size() == 1 && isdigit(input[0]) == 0) { throw std::invalid_argument("Invalid syntax"); }
    int i = 0;
    if (input[i] == '-') { ++i; }
    for (; i < input.size(); ++i) {
        if (isdigit(input[i]) == 0) { throw std::invalid_argument("Invalid syntax"); }
    }
}
std::string divStrbyTwo(const std::string& input) {
    std::string output;
    size_t idx = 0;
    int tmp = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        int result = tmp * 10 + (static_cast<int>(input[i]) - 48);
        output += std::to_string(result / 2);
        tmp = result % 2;
    }
    if (output.size() > 1 && output[0] == '0') { output.erase(0, 1); }
    return output;
}
std::string sum(std::string left, std::string right) {
    if (right.size() > left.size()) { std::swap(left, right); }
    for (size_t i = 1; i <= right.size(); ++i) {
        left[left.size() - i] = static_cast<char>(static_cast<int>(left[left.size() - i]) + static_cast<int>(right[right.size() - i]) - 48);
    }
    for (size_t i = left.size() - 1; i > 0; --i) {
        if (left[i] > '9') {
            left[i] = static_cast<char>(static_cast<int>(left[i]) - 10);
            ++left[i - 1];
        }
    }
    if (left[0] > '9') {
        left[0] = static_cast<char>(static_cast<int>(left[0]) - 10);
        left = '1' + left;
    }
    return left;
}
unsigned long long binToDec(const int* input) {
    unsigned long long output = 0;
    unsigned long long power = 1;
    for (size_t i = 0; i < 32; ++i) {
        if (input[i] == 1) { output += power; }
        power *= 2;
    }
    return output;
}
std::string binToDec(const std::string& input) {
    std::string output = "0";
    std::string power = "1";
    for (int i = static_cast<int>(input.size()) - 1; i >= 0; --i) {
        if (input[i] == '1') { output = sum(output, power); }
        power = sum(power, power);
    }
    return output;
}
// to simplify, i'll write down the digits in reverse order.
BigInteger::BigInteger() : _size(0), _arr(nullptr), _negative(false) { (*this).push_back(0); }
BigInteger::BigInteger(long long input) : _size(0), _arr(nullptr) {
    _negative = (input < 0);
    input = std::abs(input);
    while (input > 0) {
        (*this).push_back(input % base);
        input /= base;
    }
}
BigInteger::BigInteger(std::string input) : _size(0), _arr(nullptr) {
    errors(input);
    _negative = (input[0] == '-');
    if (_negative) { input.erase(input.begin()); }
    int bin[32] = {0};
    size_t idx = 0;
    while (input != "0") {
        bin[idx++] = static_cast<int>(input[input.size() - 1]) % 2;
        if (idx == 32) {
            (*this).push_back(binToDec(bin));
            for (size_t i = 0; i < 32; ++i) { bin[i] = 0; }
            idx = 0;
        }
        input = divStrbyTwo(input);
    }
    if (idx != 0) { (*this).push_back(binToDec(bin)); }
}
BigInteger::BigInteger(const BigInteger& other) {
    _size = other._size;
    _negative = other._negative;
    _arr = new unsigned long long[_size];
    for (size_t i = 0; i < _size; ++i) { _arr[i] = other._arr[i]; }
}
BigInteger::BigInteger(BigInteger&& other) noexcept : _arr(nullptr) { swap(other); }

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        BigInteger copy = other;
        swap(copy);
    }
    return *this;
}
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this != &other) { swap(other); }
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    if (abs(other) > abs(*this)) {
        if (_negative != other.negative()) {
            *this = (other.negative()) ? -(abs(other) - abs(*this)) : (abs(other) - abs(*this));
        } else {
            *this = (other.negative()) ? -(abs(other) + abs(*this)) : (abs(other) + abs(*this));
        }
        return *this;
    }
    if (_negative != other.negative()) {
        if (abs(other) == abs(*this)) {
            *this = 0;
        } else {
            BigInteger output;
            output.pop_back();
            output._negative = _negative;
            long long take = 0;
            for (size_t i = 0; i < _size; ++i) {
                long long upper = (i < _size) ? static_cast<long long>(_arr[i]) : 0;
                long long lower = (i < other.size()) ? static_cast<long long>(other._arr[i]) : 0;
                long long diff = upper - lower - take;
                if (diff < 0) {
                    diff += base;
                    take = 1;
                } else {
                    take = 0;
                }
                output.push_back(static_cast<unsigned long long>(diff));
            }
            while (output.size() > 1 && output[output.size() - 1] == 0) { output.pop_back(); }
            swap(output);
        }
    } else {
        for (size_t i = 0; i < other.size(); ++i) {
            _arr[i] += other[i];
            if (_arr[i] >= base) {
                _arr[i] %= base;
                if (i != _size - 1) {
                    ++_arr[i + 1];
                } else {
                    (*this).push_back(1);
                }
            }
        }
    }
    return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this += -other;
    return *this;
}
BigInteger& BigInteger::operator*=(const BigInteger& other) {
    BigInteger output;
    output._negative = _negative != other.negative();
    output.resize(_size + other.size());
    for (size_t i = 0; i < _size; i++) {
        for (size_t j = 0; j < other.size(); j++) {
            output._arr[i + j] += _arr[i] * other._arr[j];
            output._arr[i + j + 1] += output._arr[i + j] / base;
            output._arr[i + j] %= base;
        }
    }
    while (output.size() > 1 && output[output.size() - 1] == 0) { output.pop_back(); }
    swap(output);
    return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& other) {
    BigInteger output;
    output._negative = _negative != other.negative();
    output.resize(_size);
    for (int i = static_cast<int>(_size) - 1; i >= 0; --i) {
        unsigned long long left = 0;
        unsigned long long right = base - 1;
        while (left <= right) {
            BigInteger middle = output;
            middle._arr[i] = (left + right) / 2;
            if (middle * abs(other) <= abs(*this)) {
                left = middle._arr[i] + 1;
            } else {
                right = middle._arr[i] - 1;
            }
        }
        output._arr[i] = left - 1;
    }
    while (output.size() > 1 && output[output.size() - 1] == 0) { output.pop_back(); }
    swap(output);
    return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this - *this / other * other;
    return *this;
}

bool BigInteger::operator==(const BigInteger& other) const { return to_string(*this) == to_string(other); }
bool BigInteger::operator!=(const BigInteger& other) const { return !(*this == other); }
bool BigInteger::operator>(const BigInteger& other) const {
    if (_size > other.size()) { return true; }
    if (_size < other.size()) { return false; }
    for (int i = static_cast<int>(_size) - 1; i >= 0; --i) {
        if (_arr[i] > other[i]) { return true; }
        if (_arr[i] < other[i]) { return false; }
    }
    return false;
}
bool BigInteger::operator<(const BigInteger& other) const { return !(*this > other); }
bool BigInteger::operator<=(const BigInteger& other) const { return (*this < other) || (*this == other); }
bool BigInteger::operator>=(const BigInteger& other) const { return (*this > other) || (*this == other); }

BigInteger BigInteger::operator+(const BigInteger& other) const {
    BigInteger copy = *this;
    return copy += other;
}
BigInteger BigInteger::operator-(const BigInteger& other) const {
    BigInteger copy = *this;
    return copy -= other;
}
BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger copy = *this;
    return copy *= other;
}
BigInteger BigInteger::operator/(const BigInteger& other) const {
    BigInteger copy = *this;
    return copy /= other;
}
BigInteger BigInteger::operator%(const BigInteger& other) const {
    BigInteger copy = *this;
    return copy %= other;
}

BigInteger BigInteger::operator+() const { return *this; }
BigInteger BigInteger::operator-() const {
    BigInteger copy = *this;
    copy._negative = !copy._negative;
    return copy;
}

BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}
BigInteger BigInteger::operator++(int) {
    BigInteger copy = *this;
    *this += 1;
    return copy;
}
BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}
BigInteger BigInteger::operator--(int) {
    BigInteger copy = *this;
    *this -= 1;
    return copy;
}

BigInteger operator+(const int& left, const BigInteger& right) { return BigInteger(left) + right; }
bool operator==(const int& left, const BigInteger& right) { return BigInteger(left) == right; }
BigInteger abs(const BigInteger& other) { return (other.negative()) ? -other : other; }
std::string to_string(const BigInteger& other) {
    std::string output;
    for (int i = static_cast<int>(other.size() - 1); i >= 0; --i) {
        unsigned long long num = other[i];
        std::string tmp(32, '0');
        size_t idx = 32;
        while (num > 0) {
            tmp[--idx] = static_cast<char>(static_cast<int>(num % 2) + 48);
            num /= 2;
        }
        output += tmp;
    }
    if (other.negative() && binToDec(output) == "0") { return "0"; }
    return (other.negative()) ? "-" + binToDec(output) : binToDec(output);
}
std::ostream& operator<<(std::ostream& output, const BigInteger& other) {
    output << to_string(other);
    return output;
}