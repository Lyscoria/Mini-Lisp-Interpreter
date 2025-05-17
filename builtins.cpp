#include ".\builtins.h"
#include <iostream>

std::unordered_map<std::string, BuiltinFuncType*> builtinFuncList = {
    std::make_pair("+", add), std::make_pair("print", print)};

ValuePtr add(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    for (auto expr : params) {
        std::cout << expr->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}
