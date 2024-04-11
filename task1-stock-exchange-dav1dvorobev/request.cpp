#include "request.h"

size_t request::allObjects = 1;

bool date_cheak(const int* date){
    int year = date[0];
    int month = date[1];
    int day = date[2];
    int hour = date[3];
    int minute = date[4];
    int second = date[5];
    if(year < 2000 || year > 9999){return false;}
    if(month < 1 || month > 12){
        std::cout << "Month value is invalid: " << month << std::endl;
        return false;
    }
    if(day < 1 || day > 31){
        std::cout << "Day value is invalid: " << day << std::endl;
        return false;
    }
    if(month == 2){
        if(day > 29 || (day == 29 && year % 4 != 0)){
            std::cout << "Day value is invalid: " << day << std::endl;
            return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11){
        if (day > 30){
            std::cout << "Day value is invalid: " << day << std::endl;
            return false;
        }
    }
    if(hour < 0 || hour > 24){
        std::cout << "Time value is invalid: " 
        << std::setfill('0') << std::setw(2) << hour
        << ':' << std::setw(2) << minute
        << ':' << std::setw(2) << second << std::endl;
        return false;
    }
    if(minute < 0 || minute > 59){
        std::cout << "Time value is invalid: " 
        << std::setfill('0') << std::setw(2) << hour
        << ':' << std::setw(2) << minute
        << ':' << std::setw(2) << second << std::endl;
        return false;
    }
    if(second < 0 || second > 59){
        std::cout << "Time value is invalid: " 
        << std::setfill('0') << std::setw(2) << hour
        << ':' << std::setw(2) << minute
        << ':' << std::setw(2) << second << std::endl;
        return false;
    }
    return true;
}
request::request(const request& other){
    _id = other.id();
    for(size_t i = 0; i < 6; ++i){
        _date[i] = other.date(i);
    }
    strncpy(_ticker, other._ticker, 8);
    strncpy(_action, other._action, 8);
    _price = other.price();
    _counter = other._counter;
}
std::istream& operator>>(std::istream& input, request& other){
    other._id = request::allObjects++;

    bool status = true;
    char format = '\0';
    for(int i = 0; i < 3; ++i){
        input >> other._date[i];
        if(i < 2){
            format = static_cast<char>(input.get());
            status = (format != '.')? false : status;
        }
    }
    if(!status){
        std::cout << "Wrong date format: " << std::setfill('0')
        << std::setfill('0') << std::setw(2) << other._date[0] 
        << format << std::setw(2) << other._date[1] 
        << format << std::setw(2) << other._date[2] << std::endl;
    }
    for(int i = 3; i < 6; ++i){
        input >> other._date[i];
        input.ignore();  
    }
    std::swap(other._date[0], other._date[2]);
    status = (status)? date_cheak(other._date) : status;

    while(input.peek() == ' '){input.ignore();}
    input.getline(other._ticker, 8, ',');
    while(input.peek() == ' '){input.ignore();}
    input.getline(other._action, 8, ',');

    if(!(strcmp(other._action, "buy") == 0 || strcmp(other._action, "sell") == 0)){
        std::cout << "Unknown command: " << other.action() << std::endl;
        status = false;
    }

    input >> other._counter;
    input.ignore();
    input >> other._price;

    if(!status){
        other._id = 0;
        --request::allObjects;
    }

    return input;
}
std::ostream& operator<<(std::ostream& output, const request& other){
    for(size_t i = 0; i < 6; ++i){
        output << std::setfill('0') << std::setw(2) << other.date(i);
        output << ((i == 2)? ' ' : (i == 0 || i == 1)? '.': (i == 5)? ',': ':');
    }
    output << ' ' << other.ticker() << ", " << other.action() << ", "
    << other._counter << ", " << std::fixed << std::setprecision(2) << other.price();
    return output;
}
bool request::operator>(const request& other) const{
    for(size_t i = 0; i < 6; ++i){
        if(_date[i] > other._date[i]){return true;}
        if(_date[i] < other._date[i]){return false;}
    }
    return false;
}
request& request::operator=(const request& other){
    if(this != &other){
        request copy = other;
        swap(*this, copy);
    }
    return *this;
}