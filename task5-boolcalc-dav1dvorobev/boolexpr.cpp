#include "boolexpr.h"

size_t priority(const char& input) {
    switch (input) {
        case '~': return 9;
        case '&': return 8;
        case 'v': return 7;
        case '+': return 6;
        case '>': return 5;
        case '<': return 4;
        case '=': return 3;
        case '|': return 2;
        case '^': return 1;
        default: return 0;
    }
}
void InfixFilter(const char* input, char* output) {
    size_t j = 0;
    for (size_t i = 0; i < strlen(input); ++i) {
        if (input[i] == 'x' && isdigit(input[i + 1]) == 0) { throw -1; }
        if (input[i] != ' ' && input[i] != '\r') { output[j++] = input[i]; }
    }
    output[j] = '\0';
    for (int i = 0; i < static_cast<int>(strlen(output)) - 2; ++i) {
        if (output[i] == 'x' && priority(output[i + 2]) == 0 && output[i + 2] != ')') { throw -1; }
    }
}
void Infix2Postfix(const char* input, char* output) {
    std::stack<char> operators;
    size_t j = 0;
    for (size_t i = 0; i < strlen(input); ++i) {
        if (isalnum(input[i]) != 0 && input[i] != 'v') {
            output[j++] = input[i];
        } else if (input[i] == '(') {
            operators.push(input[i]);
        } else if (input[i] == ')') {
            while (operators.top() != '(') {
                output[j++] = operators.top();
                operators.pop();
            }
            operators.pop();
        } else {
            while (!operators.empty() && operators.top() != '(' && priority(operators.top()) > priority(input[i])) {
                output[j++] = operators.top();
                operators.pop();
            }
            operators.push(input[i]);
        }
    }
    while (!operators.empty()) {
        output[j++] = operators.top();
        operators.pop();
    }
    output[j] = '\0';
}
FormulaNode* BooleanExpression::Postfix2Tree(const char* input) {
    FormulaNode* lhs;
    FormulaNode* rhs;
    std::stack<FormulaNode*> temp;
    for (size_t i = 0; i < strlen(input); ++i) {
        switch (input[i]) {
            case '~':
                rhs = temp.top();
                temp.pop();
                temp.push(new NegativeNode(rhs));
                break;
            case '&':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new ConjunctionNode(lhs, rhs));
                break;
            case 'v':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new DisjunctionNode(lhs, rhs));
                break;
            case '+':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new OplusNode(lhs, rhs));
                break;
            case '>':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new ImplicationNode(lhs, rhs));
                break;
            case '<':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new NegativeImplicationNode(lhs, rhs));
                break;
            case '=':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new EquivNode(lhs, rhs));
                break;
            case '|':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new ShefferStrokeNode(lhs, rhs));
                break;
            case '^':
                rhs = temp.top();
                temp.pop();
                lhs = temp.top();
                temp.pop();
                temp.push(new PierceArrowNode(lhs, rhs));
                break;
            case 'x': {
                _vars[input[++i]] = false;
                for (const auto& iter : _vars) {
                    if (iter.first == input[i]) { temp.push(new ParamNode(&iter)); }
                }
                break;
            }
            default: temp.push(new BoolNode(input[i] == '1'));
        }
    }
    return temp.top();
}
bool isMonotonic(const std::vector<bool>& input) {
    if (input.size() <= 1) { return true; }
    long mid = static_cast<long>(input.size()) / 2;
    for (long i = 0; i < mid; ++i) {
        if (static_cast<int>(input[mid + i]) < static_cast<int>(input[i])) { return false; }
    }
    return isMonotonic(std::vector<bool>(input.begin(), input.begin() + mid)) &&
           isMonotonic(std::vector<bool>(input.begin() + mid, input.end()));
}
void BooleanExpression::Init() {
    TruthTable.resize(1 << _vars.size());
    for (size_t i = 0; i < TruthTable.size(); ++i) {
        size_t power = _vars.size();
        for (auto& iter : _vars) { iter.second = (i & (1 << --power)) != 0; }
        TruthTable[i] = _root->calc();
    }
    PostClasify.T0 = !TruthTable[0];
    PostClasify.T1 = TruthTable[TruthTable.size() - 1];
    PostClasify.M = isMonotonic(TruthTable);
    for (size_t i = 0; i < TruthTable.size(); ++i) {
        PostClasify.S = PostClasify.S && (TruthTable[i] == !TruthTable[TruthTable.size() - 1 - i]);
    }
}

BooleanExpression::BooleanExpression() : _root(nullptr) {}
BooleanExpression::BooleanExpression(const char* input) : _root(nullptr) {
    size_t lenght = strlen(input) + 1;
    char* str_infix = new char[lenght];
    char* str_postfix = new char[lenght];
    try {
        InfixFilter(input, str_infix);
        Infix2Postfix(str_infix, str_postfix);
        _root = Postfix2Tree(str_postfix);
        Init();
    } catch (...) {
        delete[] str_infix;
        delete[] str_postfix;
        throw;
    }
    delete[] str_infix;
    delete[] str_postfix;
}
BooleanExpression::BooleanExpression(BooleanExpression&& other) noexcept : _root(nullptr) {
    std::swap(_root, other._root);
    std::swap(_vars, other._vars);
    std::swap(TruthTable, other.TruthTable);
    std::swap(PostClasify, other.PostClasify);
}

std::vector<bool> PascalTriangle(std::vector<bool> TruthTable) {
    std::vector<bool> output(TruthTable.size());
    for (size_t i = 0; i < output.size(); ++i) {
        output[i] = TruthTable[0];
        for (size_t j = 0; j < TruthTable.size() - 1; ++j) { TruthTable[j] = TruthTable[j] != TruthTable[j + 1]; }
        TruthTable.pop_back();
    }
    return output;
}
BooleanExpression BooleanExpression::cnf() {
    FormulaNode* cnf = nullptr;
    const size_t size = 1 << _vars.size();
    for (size_t i = 0; i < size; ++i) {
        FormulaNode* temp = nullptr;
        size_t power = _vars.size();
        for (auto& iter : _vars) {
            iter.second = (i & (1 << --power)) != 0;
            if (temp == nullptr) {
                temp = new ParamNode(&iter);
                if (iter.second) { temp = new NegativeNode(temp); }
            } else {
                temp = (iter.second) ? new DisjunctionNode(temp, new NegativeNode(new ParamNode(&iter)))
                                     : new DisjunctionNode(temp, new ParamNode(&iter));
            }
        }
        if (!_root->calc()) {
            cnf = (cnf == nullptr) ? temp : new ConjunctionNode(cnf, temp);
        } else {
            delete temp;
        }
    }
    BooleanExpression output((cnf != nullptr) ? std::string(*cnf).c_str() : "0");
    delete cnf;
    return output;
}
BooleanExpression BooleanExpression::dnf() {
    FormulaNode* dnf = nullptr;
    const size_t size = 1 << _vars.size();
    for (size_t i = 0; i < size; ++i) {
        FormulaNode* temp = nullptr;
        size_t power = _vars.size();
        for (auto& iter : _vars) {
            iter.second = (i & (1 << --power)) != 0;
            if (temp == nullptr) {
                temp = new ParamNode(&iter);
                if (!iter.second) { temp = new NegativeNode(temp); }
            } else {
                temp = (!iter.second) ? new ConjunctionNode(temp, new NegativeNode(new ParamNode(&iter)))
                                      : new ConjunctionNode(temp, new ParamNode(&iter));
            }
        }
        if (_root->calc()) {
            dnf = (dnf == nullptr) ? temp : new DisjunctionNode(dnf, temp);
        } else {
            delete temp;
        }
    }
    BooleanExpression output((dnf != nullptr) ? std::string(*dnf).c_str() : "0");
    delete dnf;
    return output;
}
BooleanExpression BooleanExpression::zhegalkin() {
    const std::vector<bool>& CopyPascalTriangle = PascalTriangle(TruthTable);
    FormulaNode* zhegalkin = nullptr;
    const size_t size = 1 << _vars.size();
    for (size_t i = 0; i < size; ++i) {
        FormulaNode* temp = nullptr;
        size_t power = _vars.size();
        size_t counter = 0;
        for (auto& iter : _vars) {
            iter.second = (i & (1 << --power)) != 0;
            if (iter.second) {
                if (temp == nullptr) {
                    temp = new ParamNode(&iter);
                } else {
                    temp = new ConjunctionNode(temp, new ParamNode(&iter));
                }
                ++counter;
            }
        }
        if (temp == nullptr) { temp = new BoolNode(true); }
        if (CopyPascalTriangle[i]) {
            zhegalkin = (zhegalkin == nullptr) ? temp : new OplusNode(zhegalkin, temp);
            if (counter > 1) { PostClasify.L = false; }
        } else {
            delete temp;
        }
    }
    BooleanExpression output((zhegalkin != nullptr) ? std::string(*zhegalkin).c_str() : "0");
    delete zhegalkin;
    return output;
}

bool isFullSystem(const std::vector<BooleanExpression>& input) {
    POST output;
    for (size_t i = 0; i < input.size(); ++i) {
        output.T0 = output.T0 && input[i].PostClasify.T0;
        output.T1 = output.T1 && input[i].PostClasify.T1;
        output.L = output.L && input[i].PostClasify.L;
        output.M = output.M && input[i].PostClasify.M;
        output.S = output.S && input[i].PostClasify.S;
    }
    return !static_cast<bool>(output);
}