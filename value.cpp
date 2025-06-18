#include "./value.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>
#include <algorithm>
#include <ranges>
#include "./eval_env.h"

std::string Value::getType() const {
    if (isNumber()) {
        return "Number";
    }
    if (isBoolean()) {
        return "Boolean";
    }
    if (isSymbol()) {
        return "Symbol";
    }
    if (isString()) {
        return "String";
    }
    if (isPair()) {
        return "Pair";
    }
    if (isNil()) {
        return "Nil";
    }
    if (isProcedure()) {
        return "Procedure";
    }
    return "Other";
}

std::string BooleanValue::toString(bool toDisplay) const {
    if (value == 0) {
        return "#f";
    } else {
        return "#t";    
    }
}

std::string NumericValue::toString(bool toDisplay) const {
    if (value == int(value)) {
        return std::to_string(int(value));
    } else {
        return std::to_string(value);
    }
}

std::string StringValue::toString(bool toDisplay) const {
    std::ostringstream oss;
    oss << std::quoted(value);
    std::string result = oss.str();
    return result;
}

std::string NilValue::toString(bool toDisplay) const {
    if (toDisplay) {
        return "()";
    } else {
        return "";
    }
}

std::string SymbolValue::toString(bool toDisplay) const {
    return value;
}

std::string PairValue::toString(bool toDisplay) const {
    std::string result;
    result += car->toString(1);
    if (typeid(*cdr) != typeid(NilValue) &&
        typeid(*cdr) != typeid(PairValue)) {
        result += (" . " + cdr->toString(0));
    } else if (typeid(*cdr) == typeid(PairValue)) {
        result += (" " + cdr->toString(0));
    }
    if (toDisplay) {
        return "(" + result + ")";
    } else {
        return result;
    }
}

std::string BuiltinProcValue::toString(bool toDisplay) const {
    return "#<procedure>";
}

std::string LambdaValue::toString(bool toDisplay) const {
    return "#<procedure>";
}

bool BooleanValue::getValue() const {
    return value;
}

double NumericValue::getValue() const {
    return value;
}

std::string StringValue::getValue() const {
    return value;
}

std::string SymbolValue::getValue() const {
    return value;
}

ValuePtr PairValue::getCdr() const {
    return cdr;
}

ValuePtr PairValue::getCar() const {
    return car;
}

bool Value::isNil() const {
    return false;
}

bool NilValue::isNil() const {
    return true;
}

bool Value::isPair() const {
    return false;
}

bool PairValue::isPair() const {
    return true;
}

bool Value::isNumber() const {
    return false;
}

bool NumericValue::isNumber() const {
    return true;
}

bool Value::isInt(){
    if (!this->isNumber()) {
        return false;
    }
    auto value = this->asNumber().value();
    if ((int)value == value) {
        return true;
    }
    return false;
}

bool Value::isBoolean() const {
    return false;
}

bool BooleanValue::isBoolean() const {
    return true;
}

bool Value::isSymbol() const {
    return false;
}

bool SymbolValue::isSymbol() const {
    return true;
}

bool Value::isString() const {
    return false;
}

bool StringValue::isString() const {
    return true;
}

bool Value::isSelfEvaluating() const {
    if (isBoolean() || isNumber() || isString() || isProcedure()) {
        return true;
    } else {
        return false;
    }
}

bool Value::isProcedure() const {
    return false;
}

bool BuiltinProcValue::isProcedure() const {
    return true;
}

bool LambdaValue::isProcedure() const {
    return true;
}

bool Value::isList() {
    if (isNil()) {
        return true;
    } else if (!isPair()) {
        return false;
    } else {
        return this->getCdr()->isList();
    }
}

std::optional<double> Value::asNumber() {
    return std::nullopt;
}

std::optional<double> NumericValue::asNumber() {
    return value;
}

std::optional<std::string> Value::asSymbol() {
    return std::nullopt;
}

std::optional<std::string> SymbolValue::asSymbol() {
    return value;
}

std::optional<bool> Value::asBoolean() {
    return std::nullopt;
}

std::optional<bool> BooleanValue::asBoolean() {
    return value;
}

std::optional<std::string> Value::asString() {
    return std::nullopt;
}

std::optional<std::string> StringValue::asString() {
    return value;
}

ValuePtr BuiltinProcValue::call(const std::vector<ValuePtr>& vec, EvalEnv& env) {
    return func(vec, env);
}

std::shared_ptr<Value> Value::getCdr() const {
    return std::make_shared<NilValue>();
}

std::shared_ptr<Value> Value::getCar() const {
    return std::make_shared<NilValue>();
}

ValuePtr Value::call(const std::vector<ValuePtr>&, EvalEnv& env) {
    return std::make_shared<NilValue>();
}
ValuePtr Value::apply(const std::vector<ValuePtr>& args) {
    return std::make_shared<NilValue>();
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> newEnv = env->createChild(params, args);
    for (auto& expr : body) {
        newEnv->eval(expr);
    }
    return newEnv->eval(*(body.end() - 1));
}

std::vector<ValuePtr> Value::toVector() {
    std::vector<ValuePtr> result;
    if (isPair()) {
        /*
        PairValue* cur = dynamic_cast<PairValue*>(this);
        while (cur != nullptr) {
            result.push_back(cur->getCar());
            auto next = dynamic_cast<PairValue*>(cur->getCdr().get());
            if (next != nullptr) {
                cur = next;
            } else {
                if (!cur->getCdr()->isNil()) {
                    result.push_back(cur->getCdr());
                }
                break;
            }
        }
        */
        ValuePtr cur = shared_from_this();
        while (cur->isPair()) {
            result.push_back(cur->getCar());
            auto next = cur->getCdr();
            if (next->isPair()) {
                cur = next;
            }
            else {
                if (!cur->getCdr()->isNil()) {
                    result.push_back(cur->getCdr());
                }
                break;
            }
        }
    } else if (!isNil()) {
        result.push_back(shared_from_this());
    }
    return result;
}

ValuePtr Value::toList(std::vector<ValuePtr> vec) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto it = vec.rbegin(); it != vec.rend(); it++) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}
