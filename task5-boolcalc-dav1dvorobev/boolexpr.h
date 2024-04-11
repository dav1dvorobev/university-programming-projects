#ifndef BOOLEXPR
#define BOOLEXPR

#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include "formulanodes.h"

struct POST {
    bool T0 = true;
    bool T1 = true;
    bool L = true;
    bool M = true;
    bool S = true;
    operator bool() const { return T0 || T1 || L || M || S; }
};
class BooleanExpression {
    FormulaNode* _root;
    std::map<char, bool> _vars;
    void Init();
    std::vector<bool> TruthTable;
    FormulaNode* Postfix2Tree(const char*);
public:
    POST PostClasify;
    BooleanExpression();
    BooleanExpression(const char*);
    BooleanExpression(BooleanExpression&&) noexcept;

    BooleanExpression cnf();
    BooleanExpression dnf();
    BooleanExpression zhegalkin();

    size_t size() const { return TruthTable.size(); }
    bool operator[](const size_t& idx) const {
        if (idx < TruthTable.size()) { return TruthTable[idx]; }
        throw std::out_of_range("Index is out of range");
    }
    operator std::string() const { return std::string(*_root); }

    ~BooleanExpression() {
        delete _root;
        _root = nullptr;
    }
};
bool isFullSystem(const std::vector<BooleanExpression>&);

#endif // BOOLEXPR