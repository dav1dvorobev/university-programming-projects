#ifndef FORMULANODES
#define FORMULANODES

#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "matrix.h"


class FormulaNode {
public:
    virtual matrix get() const = 0;
    virtual std::string str() const = 0;
    virtual ~FormulaNode() {}
};
class BracketsNode : public FormulaNode {
    const char bracket;
public:
    BracketsNode(const char& other) : bracket(other){};
    matrix get() const override { return 0; }
    std::string str() const override { return std::string(1, bracket); };
};
class NumNode : public FormulaNode {
    const matrix num;
public:
    NumNode(const matrix& x) : num(x) {}
    matrix get() const override { return num; };
    std::string str() const override { return ""; }
};
namespace global {
    std::map<std::string, matrix> variables;
}
class ParamNode : public FormulaNode {
    std::string param;
public:
    ParamNode(const std::string& name) : param(name) {}
    matrix get() const override { return global::variables[param]; }
    std::string str() const override { return param; }
};

#endif // FORMULANODES