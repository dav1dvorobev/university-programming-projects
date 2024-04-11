#ifndef FORMULANODES
#define FORMULANODES

#include <iostream>

class FormulaNode {
public:
    virtual bool calc() const = 0;
    virtual operator std::string() const = 0;
    virtual ~FormulaNode() {}
};
class BoolNode : public FormulaNode {
    const bool _var;
public:
    BoolNode(const bool& other) : _var(other) {}
    bool calc() const override { return _var; }
    operator std::string() const override { return (_var) ? "1" : "0"; }
};
class ParamNode : public FormulaNode {
    const std::pair<const char, bool>* _var;
public:
    ParamNode(const std::pair<const char, bool>* other) : _var(other) {}
    bool calc() const override { return _var->second; }
    operator std::string() const override { return std::string("x") + _var->first; }
};
/*/-----------------UnaryNodes-----------------/*/
class UnaryNode : public FormulaNode {
protected:
    FormulaNode* _var;
public:
    UnaryNode(FormulaNode* other) : _var(other) {}
    ~UnaryNode() {
        delete _var;
        _var = nullptr;
    }
};
class NegativeNode : public UnaryNode {
public:
    NegativeNode(FormulaNode* other) : UnaryNode(other) {}
    bool calc() const override { return !_var->calc(); }
    operator std::string() const override { return '~' + std::string(*_var); }
};
/*/-----------------BinaryNodes-----------------/*/
class BinaryNode : public FormulaNode {
protected:
    FormulaNode* _lhs;
    FormulaNode* _rhs;
public:
    BinaryNode(FormulaNode* lhs, FormulaNode* rhs) : _lhs(lhs), _rhs(rhs) {}
    ~BinaryNode() {
        delete _lhs;
        delete _rhs;
        _lhs = _rhs = nullptr;
    }
};
class ConjunctionNode : public BinaryNode {
public:
    ConjunctionNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return _lhs->calc() && _rhs->calc(); }
    operator std::string() const override {
        if (_lhs != nullptr && _rhs != nullptr) { return '(' + std::string(*_lhs) + " & " + std::string(*_rhs) + ')'; }
        return "";
    }
};
class DisjunctionNode : public BinaryNode {
public:
    DisjunctionNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return _lhs->calc() || _rhs->calc(); }
    operator std::string() const override {
        if (_lhs != nullptr && _rhs != nullptr) { return '(' + std::string(*_lhs) + " v " + std::string(*_rhs) + ')'; }
        return "";
    }
};
class OplusNode : public BinaryNode {
public:
    OplusNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return _lhs->calc() != _rhs->calc(); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " + " + std::string(*_rhs) + ')'; }
};
class ImplicationNode : public BinaryNode {
public:
    ImplicationNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return static_cast<int>(_lhs->calc()) <= static_cast<int>(_rhs->calc()); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " > " + std::string(*_rhs) + ')'; }
};
class NegativeImplicationNode : public BinaryNode {
public:
    NegativeImplicationNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return static_cast<int>(_lhs->calc()) >= static_cast<int>(_rhs->calc()); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " < " + std::string(*_rhs) + ')'; }
};
class EquivNode : public BinaryNode {
public:
    EquivNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return _lhs->calc() == _rhs->calc(); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " = " + std::string(*_rhs) + ')'; }
};
class ShefferStrokeNode : public BinaryNode {
public:
    ShefferStrokeNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return !(_lhs->calc() && _rhs->calc()); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " | " + std::string(*_rhs) + ')'; }
};
class PierceArrowNode : public BinaryNode {
public:
    PierceArrowNode(FormulaNode* lhs, FormulaNode* rhs) : BinaryNode(lhs, rhs) {}
    bool calc() const override { return !(_lhs->calc() || _rhs->calc()); }
    operator std::string() const override { return '(' + std::string(*_lhs) + " ^ " + std::string(*_rhs) + ')'; }
};

#endif // FORMULANODES