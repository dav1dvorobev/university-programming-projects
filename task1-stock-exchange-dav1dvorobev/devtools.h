#ifndef DEVTOOLS
#define DEVTOOLS

#include <cstring>
#include <iostream>

#include "request.h"

char* upper(const char* other) {
    const size_t size = strlen(other);
    char* output = new char[size + 1];
    for (size_t i = 0; i < size; ++i) { output[i] = static_cast<char>(toupper(other[i])); }
    output[size] = '\0';
    return output;
}
void sort(request* database, const size_t& size, const char* key = "") {
    for (size_t i = 1; i < size; ++i) {
        int sorted = static_cast<int>(i) - 1;
        while (sorted > -1 && ((strcmp(key, "date") == 0) ? database[sorted] > database[sorted + 1]
                                                          : database[sorted].price() < database[sorted + 1].price())) {
            swap(database[sorted], database[sorted + 1]);
            --sorted;
        }
    }
}
void processing(request* database, const size_t& size) {
    char* ticker = upper(database[size - 1].ticker());
    while (database[size - 1]._counter > 0) {
        size_t idx = size - 1;
        for (size_t i = 0; i < size - 1; ++i) {
            char* cur_ticker = upper(database[i].ticker());
            if (strcmp(ticker, cur_ticker) == 0 && strcmp(database[size - 1].action(), database[i].action()) != 0 && database[i]._counter > 0) {
                if (strcmp(database[size - 1].action(), "sell") == 0 && database[i].id() != 0) {
                    if (database[i].price() >= database[idx].price()) { idx = i; }
                }
                if (strcmp(database[size - 1].action(), "buy") == 0 && database[i].id() != 0) {
                    if (database[i].price() <= database[idx].price()) { idx = i; }
                }
            }
            delete[] cur_ticker;
        }
        if (idx != size - 1) {
            --database[size - 1]._counter;
            --database[idx]._counter;
        } else {
            break;
        }
    }
    delete[] ticker;
}
#endif // DEVTOOLS