#include "./value.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>
#include <algorithm>
#include <ranges>
#include "./eval_env.h"

std::string BooleanValue::toString(bool toDisplay) const {
    if (val == 0)
        return "#f";
    else
        return "#t";
}

std::string NumericValue::toString(bool toDisplay) const {
    if (val == static_cast<int>(val))
        return std::to_string(static_cast<int>(val));
    else
        return std::to_string(val);
}

double NumericValue::getValue() const {
    return this->val;
}

std::string StringValue::toString(bool toDisplay) const {
    std::ostringstream oss;
    oss << std::quoted(val);
    std::string result = oss.str();
    return result;
}

std::string NilValue::toString(bool toDisplay) const {
    if (toDisplay)
        return "()";
    else
        return "";
}

std::string SymbolValue::toString(bool toDisplay) const {
    return val;
}

std::string PairValue::toString(bool toDisplay) const {
    std::string result;
    result += car->toString(0);
    if (typeid(*cdr) != typeid(NilValue) &&
        typeid(*cdr) != typeid(PairValue))
        result += (" . " + cdr->toString(0));
    else if (typeid(*cdr) == typeid(PairValue))
        result += (" " + cdr->toString(0));
    if (toDisplay)
        return "(" + result + ")";
    else
        return result;    
}

ValuePtr PairValue::getCdr() const {
    return cdr;
}

ValuePtr PairValue::getCar() const {
    return car;
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

bool Value::isSelfEvaluating() {
    if (typeid(*this) == typeid(BooleanValue) ||
        typeid(*this) == typeid(NumericValue) ||
        typeid(*this) == typeid(StringValue) ||
        typeid(*this) == typeid(BuiltinProcValue)) {
        return true;
    }
    return false;
}

bool Value::isNil() {
    if (typeid(*this) == typeid(NilValue)) {
        return true;
    }
    return false;
}

bool Value::isPair() {
    if (typeid(*this) == typeid(PairValue)) {
        return true;
    }
    return false;
}

bool Value::isNumber() {
    if (typeid(*this) == typeid(NumericValue)) {
        return true;
    }
    return false;
}

bool Value::isInt() {
    if (!this->isNumber()) {
        return false;
    }
    auto val = dynamic_cast<NumericValue*>(this)->getValue();
    if ((int)val == val) {
        return true;
    }
    return false;
}

bool Value::isBoolean() {
    if (typeid(*this) == typeid(BooleanValue)) {
        return true;
    }
    return false;
}

bool BooleanValue::getValue() const {
    return val;
}

std::optional<double> Value::asNumber() {
    if (!this->isNumber()) {
        return std::nullopt;
    } else {
        auto ptr = dynamic_cast<NumericValue*>(this);
        return ptr->getValue();
    }
}

bool Value::isSymbol() {
    if (typeid(*this) == typeid(SymbolValue)) {
        return true;
    }
    return false;
}

std::optional<std::string> Value::asSymbol() {
    if (!this->isSymbol()) {
        return std::nullopt;
    } else {
        return this->toString();
    }
}

bool Value::isString() {
    if (typeid(*this) == typeid(StringValue)) {
        return true;
    }
    return false;
}

std::string StringValue::getValue() const {
    return val;
}

bool Value::isProcedure() {
    if (typeid(*this) == typeid(BuiltinProcValue)) {
        return true;
    }
    return false;
}

std::vector<ValuePtr> PairValue::toVector() {
    std::vector<ValuePtr> result;
    PairValue* cur_ptr = this;
    while (cur_ptr != nullptr) {
        result.push_back(cur_ptr->car);
        if (auto next_ptr = dynamic_cast<PairValue*>(cur_ptr->cdr.get())) {
            cur_ptr = next_ptr;
        } else {
            if (!cur_ptr->cdr->isNil()) {
                result.push_back(cur_ptr->cdr);
            }
            break;
        }
    }
    return result;
}

std::string BuiltinProcValue::toString(bool toDisplay) const {
    return "#<procedure>";
}

ValuePtr BuiltinProcValue::call(const std::vector<ValuePtr>& v) {
    return func(v);
}

std::string LambdaValue::toString(bool toDisplay) const {
    return "#<procedure>";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    std::shared_ptr<EvalEnv> newEnv = env->createChild(params, args);
    for (auto expr : body) {
        newEnv->eval(expr);
    }
    return newEnv->eval(*(body.end() - 1));
}
