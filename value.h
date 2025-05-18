#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>
#include <vector>
#include <optional>

class Value;
using ValuePtr = std::shared_ptr<Value>;

class Value : public std::enable_shared_from_this<Value> {
public:
    Value() = default;
    virtual std::string toString(bool toDisplay = 1) const = 0;
    virtual ~Value() = default;
    bool isSelfEvaluating();
    bool isBoolean();
    bool isNil();
    bool isPair();
    bool isNumber();
    std::optional<double> asNumber();
    bool isInt();
    bool isSymbol();
    std::optional<std::string> asSymbol();
    bool isString();
    bool isProcedure();
    static ValuePtr toList(std::vector<ValuePtr>, bool toInit = 1);
};

class BooleanValue : public Value {
private:
    bool val;
public:
    BooleanValue(bool val) : val{val} {}
    std::string toString(bool toDisplay = 1) const;
};

class NumericValue : public Value {
private:
    double val;
public:
    NumericValue(double val) : val{val} {}
    std::string toString(bool toDisplay = 1) const;
    double getValue() const;
};

class StringValue : public Value {
private:
    std::string val;
public:
    StringValue(std::string val) : val {val} {}
    std::string toString(bool toDisplay = 1) const;
    std::string getValue() const;
};

class NilValue : public Value {
public:
    NilValue() = default;
    std::string toString(bool toDisplay = 1) const;
};

class SymbolValue : public Value {
private:
    std::string val;
public:
    SymbolValue(std::string val) : val{val} {}
    std::string toString(bool toDisplay = 1) const;
};

class PairValue : public Value {
private:
    std::shared_ptr<Value> left;
    std::shared_ptr<Value> right;
public:
    PairValue(ValuePtr t1, ValuePtr t2) : left{t1}, right{t2} {};
    std::shared_ptr<Value> getRight() const;
    std::shared_ptr<Value> getLeft() const;
    std::string toString(bool toDisplay = 1) const;
    std::vector<ValuePtr> toVector();
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);

class BuiltinProcValue : public Value {
private:
    BuiltinFuncType* func;

public:
    BuiltinProcValue(BuiltinFuncType* func) : func {func} {}
    std::string toString(bool toDisplay = 1) const;
    ValuePtr call(const std::vector<ValuePtr>&);
};



#endif
