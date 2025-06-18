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
    virtual bool isBoolean() const;
    virtual bool isNumber() const;
    virtual bool isSymbol() const;
    virtual bool isString() const;
    virtual bool isNil() const;
    virtual bool isPair() const;
    virtual bool isProcedure() const;
    virtual bool isSelfEvaluating() const;
    virtual bool isInt();
    virtual bool isList();
    virtual std::optional<bool> asBoolean();
    virtual std::optional<double> asNumber();
    virtual std::optional<std::string> asSymbol();
    virtual std::optional<std::string> asString();
    virtual std::string toString(bool toDisplay = 1) const = 0;
    virtual std::shared_ptr<Value> getCdr() const;
    virtual std::shared_ptr<Value> getCar() const;
    static ValuePtr toList(std::vector<ValuePtr>);
    std::vector<ValuePtr> toVector();
    std::string getType() const;
    virtual ValuePtr call(const std::vector<ValuePtr>&, EvalEnv& env);
    virtual ValuePtr apply(const std::vector<ValuePtr>& args);
    virtual ~Value() = default;
};

class BooleanValue : public Value {
private:
    bool value;
public:
    BooleanValue(bool value) : value{value} {}
    bool getValue() const;
    bool isBoolean() const;
    std::optional<bool> asBoolean();
    std::string toString(bool toDisplay = 1) const;
};

class NumericValue : public Value {
private:
    double value;
public:
    NumericValue(double value) : value{value} {}
    double getValue() const;
    bool isNumber() const;
    std::optional<double> asNumber();
    std::string toString(bool toDisplay = 1) const;
};

class StringValue : public Value {
private:
    std::string value;
public:
    StringValue(std::string value) : value {value} {}
    std::string getValue() const;
    bool isString() const;
    std::optional<std::string> asString();
    std::string toString(bool toDisplay = 1) const;
};

class NilValue : public Value {
public:
    NilValue() = default;
    bool isNil() const;
    std::string toString(bool toDisplay = 1) const;
};

class SymbolValue : public Value {
private:
    std::string value;
public:
    SymbolValue(std::string value) : value{value} {}
    std::string getValue() const;
    bool isSymbol() const;
    std::optional<std::string> asSymbol();
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
    bool isPair() const;
};

using BuiltinFuncType = ValuePtr(*)(const std::vector<ValuePtr>&, EvalEnv& env);
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType func;

public:
    BuiltinProcValue(BuiltinFuncType func) : func {func} {}
    std::string toString(bool toDisplay = 1) const;
    bool isProcedure() const;
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
    bool isProcedure() const;
    ValuePtr apply(const std::vector<ValuePtr>& args);
};

#endif
