#include ".\builtins.h"
#include <iostream>
#include <cmath>
#include <vector>

const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCEDURES = {
    std::make_pair("print", print),
    std::make_pair("display", display),
    std::make_pair("exit", exit_new),
    std::make_pair("newline", newline),
    std::make_pair("atom?", atom),
    std::make_pair("boolean?", boolean),
    std::make_pair("integer?", integer),
    std::make_pair("list?", list),
    std::make_pair("number?", number),
    std::make_pair("pair?", pair),
    std::make_pair("null?", null),
    std::make_pair("procedure?", procedure),
    std::make_pair("string?", string),
    std::make_pair("symbol?", symbol),
    std::make_pair("car", car),
    std::make_pair("cdr", cdr),
    std::make_pair("cons", cons),
    std::make_pair("length", length),
    std::make_pair("list", list_new),
    std::make_pair("append", append),
    std::make_pair("+", add), 
    std::make_pair("*", multiply),
    std::make_pair("-", minus), 
    std::make_pair("/", divide),
    std::make_pair("abs", abs_new), 
    std::make_pair("expt", expt),
    std::make_pair("quotient", quotient),
    std::make_pair("remainder", remainder_new),
    std::make_pair("modulo", modulo),
    std::make_pair("=", equal),
    std::make_pair("<", less_than), 
    std::make_pair(">", greater_than), 
    std::make_pair("<=", less_than_or_equal),
    std::make_pair(">=", greater_than_or_equal),
    std::make_pair("even?", even),
    std::make_pair("odd?", odd),
    std::make_pair("zero?", zero),
};

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

ValuePtr multiply(const std::vector<ValuePtr>& params) {
    auto result = 1.0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= i->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    if (params.size() != 1 && params.size() != 2) {
        throw LispError("\"-\" should receive one or two params.");
    }
    if (params.size() == 1) {
        if (!params[0]->isNumber()) {
            throw LispError("Cannot minus a non-numeric value.");
        }
        result = -params[0]->asNumber().value();
    }
    if (params.size() == 2) {
        if (!params[0]->isNumber() || !params[1]->isNumber()) {
            throw LispError("Cannot minus between non-numeric values.");
        }
        result = params[0]->asNumber().value() - params[1]->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params) {
    auto result = 0.0;
    if (params.size() != 1 && params.size() != 2) {
        throw LispError("\"/\" should receive one or two params.");
    }
    if (params.size() == 1) {
        if (!params[0]->isNumber()) {
            throw LispError("Cannot divide a non-numeric value.");
        }
        if (params[0]->asNumber().value() == 0) {
            throw LispError("Cannot divided by zero.");
        }
        result = 1 / params[0]->asNumber().value();
    }
    if (params.size() == 2) {
        if (!params[0]->isNumber() || !params[1]->isNumber()) {
            throw LispError("Cannot divide between non-numeric values.");
        }
        if (params[1]->asNumber().value() == 0) {
            throw LispError("Cannot divided by zero.");
        }
        result = params[0]->asNumber().value() / params[1]->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"print\" should receive one param.");
    }
    std::cout << params[0]->toString() << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr equal(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"=\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare between non-numeric values.");
    }
    if (params[0]->asNumber().value() == params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr less_than(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"<\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare between non-numeric values.");
    }
    if (params[0]->asNumber().value() < params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr greater_than(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\">\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare between non-numeric values.");
    }
    if (params[0]->asNumber().value() > params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr less_than_or_equal(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"<=\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare between non-numeric values.");
    }
    if (params[0]->asNumber().value() <= params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}
ValuePtr greater_than_or_equal(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\">=\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare between non-numeric values.");
    }
    if (params[0]->asNumber().value() >= params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr even(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"even?\" should receive one param.");
    }
    if (!params[0]->isInt()) {
        throw LispError("Cannot judge a non-integral value is even or odd.");
    }
    if ((int)params[0]->asNumber().value() % 2 == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr odd(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"odd?\" should receive one param.");
    }
    if (!params[0]->isInt()) {
        throw LispError("Cannot judge a non-integral value is even or odd.");
    }
    if ((int)params[0]->asNumber().value() % 2 != 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr zero(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"zero?\" should receive one param.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot judge a non-numeric value is zero or not.");
    }
    if (params[0]->asNumber().value() == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr abs_new(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"abs\" should receive one param.");
    }
    if (!params[0]->isNumber()) {
        throw LispError("Cannot compute absolute value for a non-numeric value.");
    }
    return std::make_shared<NumericValue>(abs(params[0]->asNumber().value()));
}

ValuePtr expt(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"expt\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compute x^y for non-numeric values.");
    }
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (x == 0 && y == 0) {
        throw LispError("0^0 is not defined.");
    }
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr quotient(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"quotient\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compute quotient for non-numeric values.");
    }
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (y == 0) {
        throw LispError("Cannot divided by zero.");
    }
    int result = int(x / y);
    return std::make_shared<NumericValue>(result);
}

ValuePtr remainder_new(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"remainder\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compute remainder for non-numeric values.");
    }
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    auto z = quotient(params)->asNumber().value();
    double result = x - y * z;
    return std::make_shared<NumericValue>(result);
}

ValuePtr modulo(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"modulo\" should receive two params.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compute modulo for non-numeric values.");
    }
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (y == 0) {
        throw LispError("Cannot divided by zero.");
    }
    int z = x / y >= 0 ? int(x / y) : int(x / y - 1);
    double result = x - y * z;
    return std::make_shared<NumericValue>(result);
}

ValuePtr display(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"display\" should receive one param.");
    }
    if (params[0]->isString()) {
        std::cout << dynamic_cast<StringValue*>(params[0].get())->getValue()
                  << std::endl;
    } else {
        std::cout << params[0]->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr exit_new(const std::vector<ValuePtr>& params) {
    if (params.size() == 0) {
        std::exit(0);
    } else if (params.size() != 1) {
        throw LispError("\"exit\" should not receive more than one param.");
    } else if (!params[0]->isInt()) {
        throw LispError("Cannot exit with a non-integral value.");
    } else {
        std::exit(params[0]->asNumber().value());
    }
    return std::make_shared<NilValue>();
}

ValuePtr newline(const std::vector<ValuePtr>& params) {
    if (params.size() != 0) {
        throw LispError("\"newline\" should not receive any params.");
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr atom(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"atom?\" should receive one param.");
    }
    if (params[0]->isBoolean() || params[0]->isNumber() ||
        params[0]->isString() || params[0]->isSymbol() || params[0]->isNil()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr boolean(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"boolean?\" should receive one param.");
    }
    if (params[0]->isBoolean()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr integer(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"integer?\" should receive one param.");
    }
    if (params[0]->isInt()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr list(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"list?\" should receive one param.");
    }
    if (params[0]->isNil() || params[0]->isPair()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr number(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"number?\" should receive one param.");
    }
    if (params[0]->isNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr null(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"null?\" should receive one param.");
    }
    if (params[0]->isNil()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr pair(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"pair?\" should receive one param.");
    }
    if (params[0]->isPair()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr procedure(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"procedure?\" should receive one param.");
    }
    if (params[0]->isProcedure()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr string(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"string?\" should receive one param.");
    }
    if (params[0]->isString()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr symbol(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"symbol?\" should receive one param.");
    }
    if (params[0]->isSymbol()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr car(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"car\" should receive one param.");
    }
    if (!params[0]->isPair()) {
        throw LispError("\"car\" should receive a pair.");
    }
    return dynamic_cast<PairValue*>(params[0].get())->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"cdr\" should receive one param.");
    }
    if (!params[0]->isPair()) {
        throw LispError("\"cdr\" should receive a pair.");
    }
    return dynamic_cast<PairValue*>(params[0].get())->getCdr();
}

ValuePtr cons(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("\"cons\" should receive two params.");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr length(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("\"length\" should receive one param.");
    }
    if (!params[0]->isPair() && !params[0]->isNil()) {
        throw LispError("\"length\" should receive a list.");
    }
    if (params[0]->isNil()) {
        return std::make_shared<NumericValue>(0);
    }
    return std::make_shared<NumericValue>(dynamic_cast<PairValue*>(params[0].get())->toVector().size());
}

ValuePtr list_new(const std::vector<ValuePtr>& params) {
    if (params.size() == 0) {
        return std::make_shared<NilValue>();
    }
    std::vector<ValuePtr> next_params;
    for (auto it = params.begin() + 1; it != params.end(); it++) {
        next_params.push_back(*it);
    }
    return std::make_shared<PairValue>(params[0], list(next_params));
}

ValuePtr append(const std::vector<ValuePtr>& params) {
    std::vector<ValuePtr> all_params;
    for (auto x : params) {
        if (!x->isPair() && !x->isNil()) {
            throw LispError("Cannot append non-list values.");
        }
        if (x->isNil()) {
            continue;
        }
        auto single_params = dynamic_cast<PairValue*>(x.get())->toVector();
        all_params.insert(all_params.end(), single_params.begin(),
                          single_params.end());
    }
    return list(all_params);
}
