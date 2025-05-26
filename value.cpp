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
    if (value == static_cast<int>(value)) {
        return std::to_string(static_cast<int>(value));
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
    if (typeid(*this) == typeid(NilValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isPair() const {
    if (typeid(*this) == typeid(PairValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isNumber() const {
    if (typeid(*this) == typeid(NumericValue)) {
        return true;
    } else {
        return false;
    }
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
    if (typeid(*this) == typeid(BooleanValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isSymbol() const {
    if (typeid(*this) == typeid(SymbolValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isString() const {
    if (typeid(*this) == typeid(StringValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isSelfEvaluating() const {
    if (isBoolean() || isNumber() || isString() || isProcedure()) {
        return true;
    } else {
        return false;
    }
}

bool Value::isProcedure() const {
    if (typeid(*this) == typeid(BuiltinProcValue) ||
        typeid(*this) == typeid(LambdaValue)) {
        return true;
    } else {
        return false;
    }
}

bool Value::isList() {
    if (isNil()) {
        return true;
    } else if (!isPair()) {
        return false;
    } else {
        return dynamic_cast<PairValue*>(this)->getCdr()->isList();
    }
}

std::optional<double> Value::asNumber() {
    if (!this->isNumber()) {
        return std::nullopt;
    } else {
        return dynamic_cast<NumericValue*>(this)->getValue();
    }
}

std::optional<std::string> Value::asSymbol() {
    if (!this->isSymbol()) {
        return std::nullopt;
    } else {
        return this->toString();
    }
}

std::optional<bool> Value::asBoolean() {
    if (!this->isBoolean()) {
        return std::nullopt;
    } else {
        return dynamic_cast<BooleanValue*>(this)->getValue();
    }
}

std::optional<std::string> Value::asString() {
    if (!this->isString()) {
        return std::nullopt;
    } else {
        return dynamic_cast<StringValue*>(this)->getValue();
    }
}

ValuePtr BuiltinProcValue::call(const std::vector<ValuePtr>& vec, EvalEnv& env) {
    return func(vec, env);
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> newEnv = env->createChild(params, args);
    for (auto expr : body) {
        newEnv->eval(expr);
    }
    return newEnv->eval(*(body.end() - 1));
}

std::vector<ValuePtr> Value::toVector() {
    std::vector<ValuePtr> result;
    if (isPair()) {
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
    } else if (!isNil()) {
        result.push_back(shared_from_this());
    }
    return result;
}

ValuePtr Value::toList(std::vector<ValuePtr> vec, bool toInit) {
    if (toInit) std::reverse(vec.begin(), vec.end());
    if (vec.size() == 0) {
        return std::make_shared<NilValue>();
    }
    auto first = vec.back();
    vec.pop_back();
    return std::make_shared<PairValue>(first, toList(vec, false));
}
