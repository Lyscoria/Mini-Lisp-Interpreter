#include "./value.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>
#include <algorithm>

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
    result += left->toString(0);
    if (typeid(*right) != typeid(NilValue) &&
        typeid(*right) != typeid(PairValue))
        result += (" . " + right->toString(0));
    else if (typeid(*right) == typeid(PairValue))
        result += (" " + right->toString(0));
    if (toDisplay)
        return "(" + result + ")";
    else
        return result;    
}

ValuePtr toList(std::vector<ValuePtr> vec, bool toInit) {
    if (toInit) std::reverse(vec.begin(), vec.end());
    if (vec.size() == 0) {
        return std::make_shared<NilValue>();
    }
    auto first = vec.back();
    vec.pop_back();
    return std::make_shared<PairValue>(first, toList(vec, false));
}
