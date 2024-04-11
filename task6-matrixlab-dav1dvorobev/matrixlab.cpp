#include <fstream>
#include <map>
#include <stack>

#include "formulanodes.h"


void delete_last(std::stack<FormulaNode*>& arr) {
    delete arr.top();
    arr.pop();
}
bool isoperator(const char& chr) {
    switch (chr) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '=': return true;
        default: return false;
    }
}
enum Instruction {
    _unknown,
    _zeros,
    _ones,
    _eye,
    _linspace,
    _vertcat,
    _horzcat,
    _transpose,
    _det,
    _inv,
    _sum,
    _prod,
    _min,
    _max,
    _disp,
    _diag
};
Instruction getInstruction(const std::string& str) {
    if (str == "zeros") { return _zeros; }
    if (str == "ones") { return _ones; }
    if (str == "eye") { return _eye; }
    if (str == "linspace") { return _linspace; }
    if (str == "vertcat") { return _vertcat; }
    if (str == "horzcat") { return _horzcat; }
    if (str == "transpose") { return _transpose; }
    if (str == "det") { return _det; }
    if (str == "inv") { return _inv; }
    if (str == "sum") { return _sum; }
    if (str == "prod") { return _prod; }
    if (str == "min") { return _min; }
    if (str == "max") { return _max; }
    if (str == "disp") { return _disp; }
    if (str == "diag") { return _diag; }
    return _unknown;
}
void Prefix2matrix(const std::string& str, std::ofstream& output) {
    std::stack<FormulaNode*> arr;
    matrix other;
    for (int i = static_cast<int>(str.size()) - 1; i > 0;) {
        while (isspace(str[i]) != 0 || str[i] == '(') { --i; }
        if (str[i] == ')') {
            arr.push(new BracketsNode(str[i]));
            --i;
        } else if (isdigit(str[i]) != 0) {
            std::string digits;
            while (isdigit(str[i]) != 0) {
                digits.insert(0, std::string(1, str[i]));
                --i;
            }
            if (isalpha(str[i]) != 0) {
                --i;
                digits.insert(0, std::string(1, str[i]));
                arr.push(new ParamNode(digits));
            } else {
                arr.push(new NumNode(matrix(stoi(digits))));
            }
        } else if (isalpha(str[i]) != 0) {
            std::string token;
            while (isalpha(str[i]) != 0) {
                token.insert(0, std::string(1, str[i]));
                --i;
            }
            switch (getInstruction(token)) {
                case _zeros: {
                    int rows = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    int cols = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(zeros(rows, cols)));
                    break;
                }
                case _ones: {
                    int rows = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    int cols = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(ones(rows, cols)));
                    break;
                }
                case _eye: {
                    int rows = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    int cols = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(eye(rows, cols)));
                    break;
                }
                case _linspace: {
                    int x0 = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    int x1 = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    int n = static_cast<int>(arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(linspace(x0, x1, n)));
                    break;
                }
                case _vertcat: {
                    while (true) {
                        other = arr.top()->get();
                        delete_last(arr);
                        other = vertcat(other, arr.top()->get());
                        delete_last(arr);
                        if (arr.top()->str() == ")") {
                            delete_last(arr);
                            arr.push(new NumNode(other));
                            break;
                        }
                        arr.push(new NumNode(other));
                    }
                    break;
                }
                case _horzcat: {
                    while (true) {
                        other = arr.top()->get();
                        delete_last(arr);
                        other = horzcat(other, arr.top()->get());
                        delete_last(arr);
                        if (arr.top()->str() == ")") {
                            delete_last(arr);
                            arr.push(new NumNode(other));
                            break;
                        }
                        arr.push(new NumNode(other));
                    }
                    break;
                }
                case _transpose: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(transpose(other)));
                    break;
                }
                case _det: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(matrix(other.det())));
                    break;
                }
                case _inv: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(inv(other)));
                    break;
                }
                case _sum: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(matrix(other.sum())));
                    break;
                }
                case _prod: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(matrix(other.prod())));
                    break;
                }
                case _min: {
                    other = arr.top()->get();
                    delete_last(arr);
                    other = min(other, arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(other));
                    break;
                }
                case _max: {
                    other = arr.top()->get();
                    delete_last(arr);
                    other = max(other, arr.top()->get());
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(other));
                    break;
                }
                case _disp: {
                    other = arr.top()->get();
                    if (!arr.top()->str().empty()) {
                        if (other.rows() == 1 && other.cols() == 1) {
                            output << arr.top()->str() << " = " << other << std::endl;
                        } else {
                            output << arr.top()->str() << " = [\n" << other << "\n]\n";
                        }
                    } else {
                        if (other.rows() == 1 && other.cols() == 1) {
                            output << "ans = " << other << std::endl;
                        } else {
                            output << "ans = [\n" << other << "\n]\n";
                        }
                    }
                    delete_last(arr);
                    delete_last(arr);
                    break;
                }
                case _diag: {
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(other.diag()));
                    break;
                }
                default: {
                    if (token.size() <= 30) { arr.push(new ParamNode(token)); }
                }
            }
        } else if (isoperator(str[i])) {
            switch (static_cast<char>(str[i])) {
                case '+': {
                    other = arr.top()->get();
                    delete_last(arr);
                    other += arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    arr.push(new NumNode(other));
                    break;
                }
                case '-': {
                    other = arr.top()->get();
                    delete_last(arr);
                    if (arr.top()->str() == ")") {
                        other = -other;
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    } else {
                        other -= arr.top()->get();
                        delete_last(arr);
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    }
                    break;
                }
                case '*': {
                    if (str[i - 1] == '.') {
                        --i;
                        other = arr.top()->get();
                        delete_last(arr);
                        other = prod(other, arr.top()->get());
                        delete_last(arr);
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    } else {
                        other = arr.top()->get();
                        delete_last(arr);
                        if (other.rows() == 1 && other.cols() == 1) {
                            other = arr.top()->get() * static_cast<rational>(other);
                            delete_last(arr);
                            delete_last(arr);
                            arr.push(new NumNode(other));
                            break;
                        }
                        other *= arr.top()->get();
                        delete_last(arr);
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    }
                    break;
                }
                case '/': {
                    if (str[i - 1] == '.') {
                        --i;
                        other = arr.top()->get();
                        delete_last(arr);
                        other = div(other, arr.top()->get());
                        delete_last(arr);
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    } else {
                        other = arr.top()->get();
                        delete_last(arr);
                        other /= arr.top()->get();
                        delete_last(arr);
                        delete_last(arr);
                        arr.push(new NumNode(other));
                    }
                    break;
                }
                case '=': {
                    std::string token = arr.top()->str();
                    delete_last(arr);
                    other = arr.top()->get();
                    delete_last(arr);
                    delete_last(arr);
                    global::variables[token] = other;
                    break;
                }
            }
            --i;
        }
    }
}
int main(int argc, char** argv) {
    std::ifstream input(argv[1], std::ios::binary);
    std::ofstream output(argv[2], std::ios::binary);
    if (!input) {
        std::cout << "Err" << std::endl;
        return -1;
    }
    while (!input.eof()) {
        std::string str;
        getline(input, str);
        Prefix2matrix(str, output);
    }
    return 0;
}