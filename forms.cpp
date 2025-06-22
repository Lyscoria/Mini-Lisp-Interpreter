#include ".\forms.h"

std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    {"define", defineForm}, 
    {"quote", quoteForm},
    {"if", ifForm},         
    {"and", andForm},
    {"or", orForm},         
    {"lambda", lambdaForm},
    {"cond", condForm},     
    {"begin", beginForm},
    {"let", letForm},       
    {"quasiquote", quasiquoteForm}
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair()) {
        auto name = args[0]->getCar();
        checkArgType({"Symbol"}, {name});
        auto params = args[0]->getCdr();
        std::vector<ValuePtr> packedArgs{params};
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
    ValuePtr result;
    for (auto& arg : args) {
        result = env.eval(arg);
        if (result->asBoolean() == false) {
            return result;
        }
    }
    return result;
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(false);
    }
    ValuePtr result;
    for (auto& arg : args) {
        result = env.eval(arg);
        if (result->asBoolean() != false) {
            return result;
        }
    }
    return result;
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
    for (auto& arg : args) {
        auto clause = arg->toVector();
        checkArgNumber(1, INT_MAX, clause);
        if (clause[0]->asSymbol() == "else") {
            if (arg != *(args.end() - 1)) {
                throw SyntaxError("Unexpected \'else\'.");
            }
            if (clause.size() == 1) {
                return env.eval(clause[0]);
            }
            ValuePtr result;
            for (int i = 1; i < clause.size(); i++) {
                result = env.eval(clause[i]);
            }
            return result;
        }
        auto condition = env.eval(clause[0]);
        if (condition->asBoolean() == false) {
            continue;
        } else {
            if (clause.size() == 1) {
                return condition;
            }
            ValuePtr result;
            for (int i = 1; i < clause.size(); i++) {
                result = env.eval(clause[i]);
            }
            return result;
        }
    }
    throw LispError("Incorrect cond form.");
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result;
    for (auto& arg : args) {
        result = env.eval(arg);
    }
    return result;
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    checkArgNumber(2, INT_MAX, args);
    auto bindings = args[0];
    std::vector<std::string> params;
    std::vector<ValuePtr> values;
    for (auto& bind : bindings->toVector()) {
        auto bind_vec = bind->toVector();
        checkArgNumber(2, 2, bind_vec);
        if (!bind_vec[0]->isSymbol()) {
            throw ArgTypeError("Symbol", bind_vec[0]->getType());
        }
        params.push_back(bind_vec[0]->toString());
        values.push_back(env.eval(bind_vec[1]));
    }
    auto newEnv = env.createChild(params, values);
    ValuePtr result;
    for (int i = 1; i < args.size(); i++) {
        result = newEnv->eval(args[i]);
    }
    return result;
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
