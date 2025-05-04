#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>

class Value {
public:
    Value() = default;
    virtual std::string toString(bool toDisplay = 1) const = 0;
    virtual ~Value() = default;
};

using ValuePtr = std::shared_ptr<Value>;

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
};

class StringValue : public Value {
private:
    std::string val;
public:
    StringValue(std::string val) : val {val} {}
    std::string toString(bool toDisplay = 1) const;
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
    std::string toString(bool toDisplay = 1) const;
};

#endif
