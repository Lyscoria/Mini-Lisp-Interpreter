#include ".\builtins.h"
#include <iostream>
#include <cmath>

std::unordered_map<std::string, BuiltinFuncType*> builtinFuncList = {
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
    std::make_pair("print", print)
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
    if (!params[0]->isNumber()) {
        throw LispError("Cannot judge a non-numeric value is even or odd.");
    }
    if (!dynamic_cast<NumericValue*>(params[0].get())->isInt()) {
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
    if (!params[0]->isNumber()) {
        throw LispError("Cannot judge a non-numeric value is even or odd.");
    }
    if (!dynamic_cast<NumericValue*>(params[0].get())->isInt()) {
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
