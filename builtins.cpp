#include "./builtins.h"
#include "./eval_env.h"
#include "./forms.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <numeric>

const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_PROCEDURES = {
    {"apply", apply},
    {"print", print},
    {"display", display},
    {"displayln", displayln},
    {"error", error},
    {"exit", exit_lisp},
    {"eval", eval},
    {"newline", newline},
    {"atom?", isatom},
    {"boolean?", isboolean},
    {"integer?", isinteger},
    {"list?", islist},
    {"number?", isnumber},
    {"pair?", ispair},
    {"null?", isnull},
    {"procedure?", isprocedure},
    {"string?", isstring},
    {"symbol?", issymbol},
    {"car", car},
    {"cdr", cdr},
    {"cons", cons},
    {"length", length},
    {"list", list},
    {"append", append},
    {"map", map},
    {"filter", filter},
    {"reduce", reduce},
    {"+", add},
    {"*", multiply},
    {"-", minus},
    {"/", divide},
    {"abs", abs_lisp},
    {"expt", expt},
    {"quotient", quotient},
    {"remainder", remainder_lisp},
    {"modulo", modulo},
    {"eq?", isEq},
    {"equal?", isEqual},
    {"not", isNot},
    {"=", equal},
    {"<", less_than},
    {">", greater_than},
    {"<=", less_than_or_equal},
    {">=", greater_than_or_equal},
    {"even?", even},
    {"odd?", odd},
    {"zero?", zero}
};

const std::vector<std::string> BUILTIN_PROCEDURE_NAMES = {
    "apply",     "print",      "display",    "displayln",
    "error",     "exit",       "eval",       "newline",
    "atom\\?",   "boolean\\?", "integer\\?", "list\\?",
    "number\\?", "pair\\?",    "null\\?",    "procedure\\?",
    "string\\?", "symbol\\?",  "car",        "cdr",
    "cons",      "length",     "list",       "append",
    "map",       "filter",     "reduce",     "\\+",
    "\\*",       "-",          "/",          "abs",
    "expt",      "quotient",   "remainder",  "modulo",
    "eq\\?",     "equal\\?",   "not",        "=",
    "<",         ">",          "<=",         ">=",
    "even\\?",   "odd\\?",     "zero\\?"};

ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = std::accumulate(params.begin(), params.end(), 0.0, 
        [](double acc, const auto& param) {
            if (!param->isNumber()) {
                throw ArgTypeError("Number", param->getType());
            }
            return acc + param->asNumber().value();
        });
    return std::make_shared<NumericValue>(result);
}

ValuePtr multiply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    double result = std::accumulate(
        params.begin(), params.end(), 1.0, [](double acc, const auto& param) {
            if (!param->isNumber()) {
                throw ArgTypeError("Number", param->getType());
            }
            return acc * param->asNumber().value();
        });
    return std::make_shared<NumericValue>(result);
}

ValuePtr minus(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 0.0;
    checkArgNumber(1, 2, params);
    if (params.size() == 1) {
        checkArgType({"Number"}, params);
        result = -params[0]->asNumber().value();
    }
    if (params.size() == 2) {
        checkArgType({"Number", "Number"}, params);
        result = params[0]->asNumber().value() - params[1]->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env) {
    auto result = 0.0;
    checkArgNumber(1, 2, params);
    if (params.size() == 1) {
        checkArgType({"Number"}, params);
        if (params[0]->asNumber().value() == 0) {
            throw LispError("Division by zero.");
        }
        result = 1 / params[0]->asNumber().value();
    }
    if (params.size() == 2) {
        checkArgType({"Number", "Number"}, params);
        if (params[1]->asNumber().value() == 0) {
            throw LispError("Cannot divided by zero.");
        }
        result = params[0]->asNumber().value() / params[1]->asNumber().value();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    std::cout << params[0]->toString() << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    if (params[0]->asNumber().value() == params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr less_than(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    if (params[0]->asNumber().value() < params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr greater_than(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    if (params[0]->asNumber().value() > params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr less_than_or_equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    if (params[0]->asNumber().value() <= params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}
ValuePtr greater_than_or_equal(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    if (params[0]->asNumber().value() >= params[1]->asNumber().value()) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (!params[0]->isInt()) {
        throw ArgTypeError("Int", params[0]->getType());
    }
    if ((int)params[0]->asNumber().value() % 2 == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (!params[0]->isInt()) {
        throw ArgTypeError("Int", params[0]->getType());
    }
    if ((int)params[0]->asNumber().value() % 2 != 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    checkArgType({"Number"}, params);
    if (params[0]->asNumber().value() == 0) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr abs_lisp(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    checkArgType({"Number"}, params);
    return std::make_shared<NumericValue>(abs(params[0]->asNumber().value()));
}

ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (x == 0 && y == 0) {
        throw LispError("0^0 is not defined.");
    }
    return std::make_shared<NumericValue>(pow(x, y));
}

ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (y == 0) {
        throw LispError("Division by zero.");
    }
    int result = int(x / y);
    return std::make_shared<NumericValue>(result);
}

ValuePtr remainder_lisp(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    auto z = quotient(params, env)->asNumber().value();
    double result = x - y * z;
    return std::make_shared<NumericValue>(result);
}

ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Number", "Number"}, params);
    auto x = params[0]->asNumber().value();
    auto y = params[1]->asNumber().value();
    if (y == 0) {
        throw LispError("Division by zero.");
    }
    int z = x / y >= 0 ? int(x / y) : int(x / y - 1);
    double result = x - y * z;
    return std::make_shared<NumericValue>(result);
}

ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isString()) {
        std::cout << dynamic_cast<StringValue*>(params[0].get())->getValue()
                  << std::endl;
    } else {
        std::cout << params[0]->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr exit_lisp(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0) {
        std::exit(0);
    } else if (params.size() != 1) {
        throw ArgNumberError(0, 1, params.size());
    } else if (!params[0]->isInt()) {
        throw ArgTypeError("Int", params[0]->getType());
    } else {
        std::exit(params[0]->asNumber().value());
    }
}

ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(0, 0, params);
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr isatom(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isBoolean() || params[0]->isNumber() ||
        params[0]->isString() || params[0]->isSymbol() || params[0]->isNil()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isboolean(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isBoolean()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isinteger(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isInt()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr islist(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isList()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isnumber(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isNumber()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isnull(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isNil()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr ispair(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isPair()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isprocedure(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isProcedure()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr isstring(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isString()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr issymbol(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->isSymbol()) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    checkArgType({"Pair"}, params);
    return params[0]->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    checkArgType({"Pair"}, params);
    return params[0]->getCdr();
}

ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (!params[0]->isList()) {
        throw ArgTypeError("List", params[0]->getType());
    }
    if (params[0]->isNil()) {
        return std::make_shared<NumericValue>(0);
    }
    return std::make_shared<NumericValue>(params[0]->toVector().size());
}

ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env) {
    if (params.size() == 0) {
        return std::make_shared<NilValue>();
    }
    return std::make_shared<PairValue>(params[0], 
        list(std::vector<ValuePtr>(params.begin() + 1, params.end()), env));
}

ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env) {
    std::vector<ValuePtr> all_params;
    for (auto& param : params) {
        if (!param->isList()) {
            throw ArgTypeError("List", param->getType());
        }
        if (param->isNil()) {
            continue;
        }
        auto single_params = param->toVector();
        all_params.insert(all_params.end(), single_params.begin(),
                          single_params.end());
    }
    return list(all_params, env);
}

ValuePtr apply(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Procedure"}, params);
    if (!params[1]->isList()) {
        throw ArgTypeError("List", params[1]->getType());
    }
    auto proc = params[0];
    auto list = params[1];
    return env.apply(proc, list->toVector());
}

ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env) {
    display(params, env);
    newline({}, env);
    return std::make_shared<NilValue>();
}

ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(0, 1, params);
    if (params.size() == 0) {
        throw LispError(std::make_shared<NilValue>()->toString());
    } else {
        throw LispError(params[0]->toString());
    }
}

ValuePtr eval(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    return env.eval(params[0]);
}

ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Procedure"}, params);
    if (!params[1]->isList()) {
        throw ArgTypeError("List", params[1]->getType());
    }
    auto proc = params[0];
    auto list = params[1];
    std::vector<ValuePtr> result;
    std::ranges::transform(list->toVector(), std::back_inserter(result),
        [&](ValuePtr value) { return env.apply(proc, {value}); });
    return Value::toList(result);
}

ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Procedure"}, params);
    if (!params[1]->isList()) {
        throw ArgTypeError("List", params[1]->getType());
    }
    auto proc = params[0];
    auto list = params[1];
    auto results = list->toVector();
    results.erase(
        std::remove_if(results.begin(), results.end(),
            [&](const ValuePtr& value) {
                return isNot({env.apply(proc, {value})}, env)->asBoolean() ==true;
            }), results.end());
    return Value::toList(results);
}

ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    checkArgType({"Procedure"}, params);
    if (!params[1]->isList()) {
        throw ArgTypeError("List", params[1]->getType());
    }
    if (params[1]->isNil()) {
        throw LispError("Expected non-nil value, but received Nil.");
    }
    auto proc = params[0];
    auto list = params[1];
    if (length({list}, env)->asNumber() == 1) {
        return car({list}, env);
    } else {
        return env.apply(proc, {car({list}, env),
                      reduce({proc, cdr({list}, env)}, env)});
    }

}

ValuePtr isEq(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    if (params[0]->getType() != params[1]->getType()) {
        return std::make_shared<BooleanValue>(false);
    }
    if (params[0]->isSymbol() || params[0]->isNumber() || 
        params[0]->isBoolean() || params[0]->isNil()) {
        return std::make_shared<BooleanValue>(params[0]->toString() == params[1]->toString());
    } else {
        return std::make_shared<BooleanValue>(params[0] == params[1]);
    }
}

ValuePtr isEqual(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(2, 2, params);
    if (params[0]->getType() != params[1]->getType()) {
        return std::make_shared<BooleanValue>(false);
    }
    return std::make_shared<BooleanValue>(params[0]->toString() == params[1]->toString());
}

ValuePtr isNot(const std::vector<ValuePtr>& params, EvalEnv& env) {
    checkArgNumber(1, 1, params);
    if (params[0]->asBoolean().has_value() &&
        params[0]->asBoolean().value() == false) {
        return std::make_shared<BooleanValue>(true);
    } else {
        return std::make_shared<BooleanValue>(false);
    }
}
