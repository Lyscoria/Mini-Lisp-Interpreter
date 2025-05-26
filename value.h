#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <memory>
#include <vector>
#include <optional>

class EvalEnv;
class Value;
using ValuePtr = std::shared_ptr<Value>;

class Value : public std::enable_shared_from_this<Value> {
public:
    Value() = default;
    bool isBoolean() const;
    bool isNumber() const;
    bool isSymbol() const;
    bool isString() const;
    bool isNil() const;
    bool isPair() const;
    bool isProcedure() const;
    bool isSelfEvaluating() const;
    bool isInt();
    bool isList();
    std::optional<bool> asBoolean();
    std::optional<double> asNumber();
    std::optional<std::string> asSymbol();
    std::optional<std::string> asString();
    virtual std::string toString(bool toDisplay = 1) const = 0;
    static ValuePtr toList(std::vector<ValuePtr>, bool toInit = 1);
    std::vector<ValuePtr> toVector();
    std::string getType() const;
    virtual ~Value() = default;
};

class BooleanValue : public Value {
private:
    bool value;
public:
    BooleanValue(bool value) : value{value} {}
    bool getValue() const;
    std::string toString(bool toDisplay = 1) const;
};

class NumericValue : public Value {
private:
    double value;
public:
    NumericValue(double value) : value{value} {}
    double getValue() const;
    std::string toString(bool toDisplay = 1) const;
};

class StringValue : public Value {
private:
    std::string value;
public:
    StringValue(std::string value) : value {value} {}
    std::string getValue() const;
    std::string toString(bool toDisplay = 1) const;
};

class NilValue : public Value {
public:
    NilValue() = default;
    std::string toString(bool toDisplay = 1) const;
};

class SymbolValue : public Value {
private:
    std::string value;
public:
    SymbolValue(std::string value) : value{value} {}
    std::string getValue() const;
    std::string toString(bool toDisplay = 1) const;
};

class PairValue : public Value {
private:
    std::shared_ptr<Value> car;
    std::shared_ptr<Value> cdr;
public:
    PairValue(ValuePtr car, ValuePtr cdr) : car{car}, cdr{cdr} {};
    std::shared_ptr<Value> getCdr() const;
    std::shared_ptr<Value> getCar() const;
    std::string toString(bool toDisplay = 1) const;
};

using BuiltinFuncType = ValuePtr(*)(const std::vector<ValuePtr>&, EvalEnv& env);
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType func;

public:
    BuiltinProcValue(BuiltinFuncType func) : func {func} {}
    std::string toString(bool toDisplay = 1) const;
    ValuePtr call(const std::vector<ValuePtr>&, EvalEnv& env);
};

class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;

public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body,
                std::shared_ptr<EvalEnv> env)
        : params{params}, body{body}, env{env} {}
    std::string toString(bool toDisplay = 1) const;
    ValuePtr apply(const std::vector<ValuePtr>& args);
};

#endif
