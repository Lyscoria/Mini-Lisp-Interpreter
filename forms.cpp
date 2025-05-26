#include ".\forms.h"

std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    std::make_pair("define", defineForm), 
    std::make_pair("quote", quoteForm),
    std::make_pair("if", ifForm),
    std::make_pair("and", andForm), 
    std::make_pair("or", orForm),
    std::make_pair("lambda", lambdaForm),
    std::make_pair("cond", condForm),     
    std::make_pair("begin", beginForm),
    std::make_pair("let", letForm),
    std::make_pair("quasiquote", quasiquoteForm)
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair()) {
        auto name = dynamic_cast<PairValue*>(args[0].get())->getCar();
        checkArgType({"Symbol"}, {name});
        auto params = dynamic_cast<PairValue*>(args[0].get())->getCdr();
        std::vector<ValuePtr> packedArgs;
        packedArgs.push_back(params);
        packedArgs.insert(packedArgs.end(), args.begin() + 1, args.end());
        env.defineBinding(*name->asSymbol(), lambdaForm(packedArgs, env));
        return std::make_shared<NilValue>();
    } else {
        throw SyntaxError("Unsupported syntax.");
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    checkArgNumber(3, 3, args);
    if (env.eval(args[0])->asBoolean() == false) {
        return env.eval(args[2]);        
    } else {
        return env.eval(args[1]);
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(true);
    }
    for (auto arg : args) {
        if (env.eval(arg)->asBoolean() == false) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return env.eval(*(args.rbegin()));
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(false);
    }
    for (auto arg : args) {
        if (env.eval(arg)->asBoolean() != false) {
            return env.eval(arg);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    checkArgNumber(2, INT_MAX, args);
    std::vector<std::string> params;
    std::ranges::transform(args[0]->toVector(), std::back_inserter(params), [](ValuePtr ptr) {
                               checkArgType({"Symbol"}, {ptr});
                               return *ptr->asSymbol(); 
                            });
    return std::make_shared<LambdaValue>(params, 
        std::vector<ValuePtr>(args.begin() + 1, args.end()), env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto arg : args) {
        auto clause = arg->toVector();
        checkArgNumber(1, INT_MAX, clause);
        if (clause[0]->asSymbol() == "else") {
            if (arg != *(args.end() - 1)) {
                throw SyntaxError("Unexpected \'else\'.");
            }
            if (clause.size() == 1) {
                return env.eval(clause[0]);
            }
            for (auto expr : clause) {
                if (expr == clause[0]) continue;
                env.eval(expr);
            }
            return env.eval(*(clause.rbegin()));
        }
        if (env.eval(clause[0])->asBoolean() == false) {
            continue;
        } else {
            if (clause.size() == 1) {
                return env.eval(clause[0]);
            }
            for (auto expr : clause) {
                if (expr == clause[0]) continue;
                env.eval(expr);
            }
            return env.eval(*(clause.rbegin()));
        }
    }
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto arg : args) {
        env.eval(arg);
    }
    return env.eval(*args.rbegin());
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    checkArgNumber(2, INT_MAX, args);
    auto bindings = args[0];
    std::vector<ValuePtr> params;
    std::vector<ValuePtr> values;
    for (auto bind : bindings->toVector()) {
        checkArgNumber(2, 2, bind->toVector());
        params.push_back(bind->toVector()[0]);
        values.push_back(env.eval(bind->toVector()[1]));
    }
    std::vector<ValuePtr> packedArgs;
    packedArgs.push_back(Value::toList(params));
    packedArgs.insert(packedArgs.end(), args.begin() + 1, args.end());
    auto lambda = lambdaForm(packedArgs, env);
    return dynamic_cast<LambdaValue*>(lambda.get())->apply(values);
}

ValuePtr quasiquoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    checkArgNumber(1, 1, args);
    if (!args[0]->isPair()) {
        return args[0];
    }
    auto values = args[0]->toVector();
    for (auto& value : values) {
        if (value->isPair()) {
            auto pair = value->toVector();
            if (pair[0]->asSymbol() == "unquote") {
                value = env.eval(pair[1]);
            }
        }
    }
    return Value::toList(values);
}
