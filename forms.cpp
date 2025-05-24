#include ".\forms.h"

std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS{
    std::make_pair("define", defineForm), 
    std::make_pair("quote", quoteForm),
    std::make_pair("if", ifForm),
    std::make_pair("and", andForm), 
    std::make_pair("or", orForm),
    std::make_pair("lambda", lambdaForm)
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    //TODO: need fix
    if (auto name = args[0]->asSymbol()) {
        env.add(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair()) {
        auto name = dynamic_cast<SymbolValue*>(
                        dynamic_cast<PairValue*>(args[0].get())->getCar().get())
                        ->asSymbol();
        auto params = dynamic_cast<PairValue*>(args[0].get())->getCdr();
        std::vector<ValuePtr> newArgs;
        newArgs.push_back(params);
        newArgs.insert(newArgs.end(), args.begin() + 1, args.end());
        env.add(*name, lambdaForm(newArgs, env));
        return std::make_shared<NilValue>();
    }
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) {
        throw SyntaxError("\'if\' requires 3 arguments.");
    }
    if (dynamic_cast<BooleanValue*>(env.eval(args[0]).get()) != nullptr && 
        dynamic_cast<BooleanValue*>(env.eval(args[0]).get())->getValue() == false) {
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
        if (env.eval(arg)->isBoolean() == true && 
            dynamic_cast<BooleanValue*>(env.eval(arg).get())->getValue() == false) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return env.eval(*(args.end() - 1));
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() == 0) {
        return std::make_shared<BooleanValue>(false);
    }
    for (auto arg : args) {
        if (env.eval(arg)->isBoolean() != true || dynamic_cast<BooleanValue*>(
            env.eval(arg).get())->getValue() != false) {
            return env.eval(arg);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    //TODO: Type check
    if (args.size() < 2) {
        throw SyntaxError("\'lambda\' requires at least 2 arguments.");
    }
    std::vector<std::string> params;
    std::vector<ValuePtr> paramPtrs = dynamic_cast<PairValue*>(args[0].get())->toVector();
    for (auto ptr : paramPtrs) {
        params.push_back(*ptr->asSymbol());
    }
    return std::make_shared<LambdaValue>(params, std::vector<ValuePtr>(args.begin() + 1, args.end()));
}
