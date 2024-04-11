#include "biginteger.h"

const long long base = 1e9;

BigInteger::BigInteger() : _negative(false) { arr.push_back(0); }
BigInteger::BigInteger(long long other) {
    _negative = (other < 0);
    other = std::abs(other);
    if (other == 0) { arr.push_back(other % base); }
    while (other > 0) {
        arr.push_back(other % base);
        other /= base;
    }
}
BigInteger::BigInteger(std::string other) {
    _negative = (other[0] == '-');
    if (_negative) { other.erase(other.begin()); }
    int size = static_cast<int>(other.size());
    for (int i = size; i > 0; i -= 9) { arr.push_back((i < 9) ? stoi(other.substr(0, i)) : stoi(other.substr(i - 9, 9))); }
}
BigInteger::BigInteger(const BigInteger& other) {
    _negative = other.negative();
    arr.clear();
    arr = other.arr;
}
BigInteger::BigInteger(BigInteger&& other) noexcept {
    std::swap(arr, other.arr);
    std::swap(_negative, other._negative);
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this != &other) {
        BigInteger copy = other;
        std::swap(arr, copy.arr);
        std::swap(_negative, copy._negative);
    }
    return *this;
}
BigInteger& BigInteger::operator=(BigInteger&& other) noexcept {
    if (this != &other) {
        std::swap(arr, other.arr);
        std::swap(_negative, other._negative);
    }
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
        std::vector<long long> result;
        long long carry = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            long long left = (i < arr.size()) ? static_cast<long long>(arr[i]) : 0;
            long long right = (i < other.size()) ? static_cast<long long>(other.arr[i]) : 0;
            long long diff = left - right - carry;
            if (diff < 0) {
                diff += base;
                carry = 1;
            } else {
                carry = 0;
            }
            result.push_back(diff);
        }
        while (result.size() > 1 && result.back() == 0) { result.pop_back(); }
        arr.clear();
        for (auto x : result) { arr.push_back(static_cast<unsigned long long>(x)); }
    } else {
        for (size_t i = 0; i < other.size(); ++i) {
            arr[i] += other.arr[i];
            if (arr[i] >= base) {
                arr[i] %= base;
                if (i != arr.size() - 1) {
                    ++arr[i + 1];
                } else {
                    arr.push_back(1);
                }
            }
        }
    }
    return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this += (-other);
    return *this;
}
BigInteger& BigInteger::operator*=(const BigInteger& other) {
    _negative = _negative != other._negative;
    size_t size = arr.size() + other.size();
    std::vector<unsigned long long> result(size, 0);
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = 0; j < other.size(); j++) {
            result[i + j] += arr[i] * other.arr[j];
            result[i + j + 1] += result[i + j] / base;
            result[i + j] %= base;
        }
    }
    while (size > 1 && result[size - 1] == 0) { --size; }
    arr.clear();
    for (size_t i = 0; i < size; ++i) { arr.push_back(result[i]); }
    return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& other) {
    _negative = _negative != other.negative();
    BigInteger result;
    result.arr.resize(arr.size());
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        unsigned long long left = 0;
        unsigned long long right = base - 1;
        while (left <= right) {
            BigInteger middle = result;
            middle.arr[i] = (left + right) / 2;
            if (middle * abs(other) <= abs(*this)) {
                left = middle.arr[i] + 1;
            } else {
                right = middle.arr[i] - 1;
            }
        }
        result.arr[i] = left - 1;
    }
    while (result.size() > 1 && result.arr[result.size() - 1] == 0) { result.arr.pop_back(); }
    arr.clear();
    arr = result.arr;
    return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this - *this / other * other;
    return *this;
}

bool BigInteger::operator==(const BigInteger& other) const { return to_string(*this) == to_string(other); }
bool BigInteger::operator!=(const BigInteger& other) const { return !((*this) == other); }
bool BigInteger::operator>(const BigInteger& other) const {
    if (arr.size() > other.arr.size()) { return true; }
    if (arr.size() < other.arr.size()) { return false; }
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        if (arr[i] > other.arr[i]) { return true; }
        if (arr[i] < other.arr[i]) { return false; }
    }
    return false;
}
bool BigInteger::operator<(const BigInteger& other) const { return !(*this > other); }
bool BigInteger::operator<=(const BigInteger& other) const { return (*this < other) || (*this == other); }

BigInteger BigInteger::operator+(const BigInteger& other) const {
    BigInteger copy = *this;
    copy += other;
    return copy;
}
BigInteger BigInteger::operator-(const BigInteger& other) const {
    BigInteger copy = *this;
    copy -= other;
    return copy;
}
BigInteger BigInteger::operator*(const BigInteger& other) const {
    BigInteger copy = *this;
    copy *= other;
    return copy;
}
BigInteger BigInteger::operator/(const BigInteger& other) const {
    BigInteger copy = *this;
    copy /= other;
    return copy;
}
BigInteger BigInteger::operator%(const BigInteger& other) const {
    BigInteger copy = *this;
    copy %= other;
    return copy;
}

BigInteger BigInteger::operator-() const {
    BigInteger copy = *this;
    copy._negative = !copy.negative();
    return copy;
}

BigInteger abs(const BigInteger& other) { return (other.negative()) ? -other : other; }
std::string to_string(const BigInteger& other) {
    std::stringstream output;
    if (other.negative()) { output << '-'; }
    output << other.arr[other.size() - 1];
    for (int i = static_cast<int>(other.size()) - 2; i >= 0; --i) {
        output.width(9);
        output.fill('0');
        output << other.arr[i];
    }
    if (output.str() == "-0") { return "0"; }
    return output.str();
}
std::ostream& operator<<(std::ostream& output, const BigInteger& other) {
    output << to_string(other);
    return output;
}