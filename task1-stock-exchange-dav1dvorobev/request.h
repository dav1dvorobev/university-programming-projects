#ifndef REQUEST
#define REQUEST

#include <cstring>
#include <iomanip>
#include <iostream>

class request {
    size_t _id;
    int _date[6]; // data & time
    char _ticker[8];
    char _action[8];
    double _price;
    static size_t allObjects; // objects
public:
    size_t _counter;

    request() : _id(0){};
    request(const request&);

    bool operator>(const request&) const;
    request& operator=(const request&);

    friend std::istream& operator>>(std::istream&, request&);
    friend void swap(request& left, request& right) {
        std::swap(left._id, right._id);
        std::swap(left._date, right._date);
        std::swap(left._ticker, right._ticker);
        std::swap(left._action, right._action);
        std::swap(left._price, right._price);
        std::swap(left._counter, right._counter);
        std::swap(left._id, right._id);
    }

    void hide() { _id = 0; }
    size_t id() const { return _id; }
    int date(const size_t& idx) const { return _date[idx]; }
    request format() const {
        request copy = *this;
        std::swap(copy._date[0], copy._date[2]);
        return copy;
    }
    const char* ticker() const { return _ticker; }
    const char* action() const { return _action; }
    double price() const { return _price; }
};
std::ostream& operator<<(std::ostream&, const request&);
#endif // REQUEST