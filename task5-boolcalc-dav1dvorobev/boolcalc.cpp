#include <fstream>
#include <iostream>
#include <vector>

#include "boolexpr.h"

int main(int argc, char** argv) {
    std::ifstream input(argv[2]);
    if (!input) {
        std::cout << "Can't open file " << argv[2] << std::endl;
        return 1;
    }
    std::ofstream output(argv[3]);

    char cstr[256] = "";
    if (strcmp(argv[1], "-cnf") == 0) {
        try {
            while (!input.eof()) {
                input.getline(cstr, 256, '\n');
                if (strlen(cstr) != 0) {
                    (strcmp(cstr, "0") == 0 || strcmp(cstr, "1") == 0) ? output << cstr << '\n'
                                                                       : output << std::string(BooleanExpression(cstr).cnf()) << '\n';
                }
            }
        } catch (...) { output << "error"; }
    } else if (strcmp(argv[1], "-dnf") == 0) {
        try {
            while (!input.eof()) {
                input.getline(cstr, 256, '\n');
                if (strlen(cstr) != 0) {
                    (strcmp(cstr, "0") == 0 || strcmp(cstr, "1") == 0) ? output << cstr << '\n'
                                                                       : output << std::string(BooleanExpression(cstr).dnf()) << '\n';
                }
            }
        } catch (...) { output << "error"; }
    } else if (strcmp(argv[1], "-zh") == 0) {
        try {
            while (!input.eof()) {
                input.getline(cstr, 256, '\n');
                if (strlen(cstr) != 0) {
                    (strcmp(cstr, "0") == 0 || strcmp(cstr, "1") == 0) ? output << cstr << '\n'
                                                                       : output << std::string(BooleanExpression(cstr).zhegalkin()) << '\n';
                }
            }
        } catch (...) { output << "error"; }
    } else if (strcmp(argv[1], "-isfull") == 0) {
        std::vector<BooleanExpression> temp;
        try {
            while (!input.eof()) {
                input.getline(cstr, 256, '\n');
                if (strlen(cstr) != 0) {
                    temp.push_back(cstr);
                    if (strcmp(cstr, "0") != 0 && strcmp(cstr, "1") != 0) { temp.back().zhegalkin(); }
                }
            }
            output << (isFullSystem(temp) ? "yes" : "no");
        } catch (...) { output << "error"; }
    } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "?") == 0) {
        std::cout << "The program boolcalc provides several operations for processing a system of Boolean functions:\n\n"
                     "* boolcalc -cnf input output: Generates the Conjunctive Normal Form (CNF) for the functions from the input file and "
                     "writes the results to the output file.\n\n"
                     "* boolcalc -dnf input output: Generates the Disjunctive Normal Form (DNF) for the functions from the input file and "
                     "writes the results to the output file.\n\n"
                     "* boolcalc -zh input output: Generates the Zhegalkin Polynomial for the functions from the input file and writes the "
                     "results to the output file.\n\n"
                     "* boolcalc -isfull input output: Checks the completeness of the system of functions from the input file. If the system "
                     "is complete, it outputs 'yes' to the output file; otherwise, it outputs 'no'.\n\n"
                     "* boolcalc -h or boolcalc ?: Displays the usage help for the program."
                  << std::endl;
    } else {
        std::cout << argv[1] << " is not defined. Type -h or ? for interactive help." << std::endl;
    }
    input.close();
    output.close();
    return 0;
}