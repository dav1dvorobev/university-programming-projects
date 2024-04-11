#include <cstring>
#include <fstream>
#include <iostream>

#include "devtools.h"
#include "request.h"

request database[1024];
size_t idx = 0;

char cstr[16] = "";

int main() {
    char command[8] = "";
    while (true) {
        std::cout << "> ";
        std::cin >> command;
        if (strcmp(command, "load") == 0) {
            std::cin >> cstr;
            std::ifstream input(cstr);
            if (input) {
                while (!input.eof()) { input >> database[idx++]; }
            } else {
                std::cout << "Can not open database" << std::endl;
            }
            input.close();
        } else if (strcmp(command, "save") == 0) {
            std::cin >> cstr;
            std::ofstream output(cstr);
            bool flag = false;
            for (size_t i = 0; i < idx; ++i) {
                if (database[i].id() != 0) {
                    if (flag) {
                        output << std::endl;
                    } else {
                        flag = true;
                    }
                    output << database[i].format();
                }
            }
            output.close();
        } else if (strcmp(command, "add") == 0) {
            std::cin >> database[idx++];
            processing(database, idx);
        } else if (strcmp(command, "delete") == 0) {
            size_t input = 0;
            std::cin >> input;
            for (size_t i = 0; i < idx; ++i) {
                if (database[i].id() == input) { database[i].hide(); }
            }
        } else if (strcmp(command, "clear") == 0) {
            for (size_t i = 0; i < idx; ++i) {
                if (database[i]._counter == 0) { database[i].hide(); }
            }
        } else if (strcmp(command, "print") == 0) {
            sort(database, idx, "date");
            for (size_t i = 0; i < idx; ++i) {
                if (database[i].id() != 0) { std::cout << database[i].id() << ", " << database[i] << std::endl; }
            }
        } else if (strcmp(command, "dom") == 0) {
            std::cin >> cstr;
            char* ticker = upper(cstr);
            sort(database, idx, "price");
            for (size_t i = 0; i < idx; ++i) {
                char* cur_ticker = upper(database[i].ticker());
                if (database[i].id() != 0 && strcmp(ticker, cur_ticker) == 0) {
                    std::cout << database[i].action() << ", " << std::fixed << std::setprecision(2) << database[i].price() << ", "
                              << database[i]._counter << ", ";
                    for (size_t j = 0; j < 6; ++j) {
                        std::cout << std::setfill('0') << std::setw(2) << database[i].date(j);
                        std::cout << ((j == 2) ? ' ' : (j == 0 || j == 1) ? '.' : (j == 5) ? '\n' : ':');
                    }
                }
                delete[] cur_ticker;
            }
            delete[] ticker;
        } else if (strcmp(command, "export") == 0) {
            std::cin >> cstr;
            std::ofstream output(cstr);
            sort(database, idx, "date");
            for (size_t i = 0; i < idx; ++i) {
                if (database[i].id() != 0) { output << database[i] << std::endl; }
            }
            output.close();
        } else if (strcmp(command, "quit") == 0) {
            return 0;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
            std::cin.ignore(1e9, '\n');
        }
    }

    return 0;
}