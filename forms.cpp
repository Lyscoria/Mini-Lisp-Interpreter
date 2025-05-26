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
        env.defineBinding(*name, env.eval(args[1]));
        return std::make_shared<NilValue>();
    } else if (args[0]->isPair()) {
        auto name = dynamic_cast<PairValue*>(args[0].get())->getCar().get()->asSymbol();
        auto params = dynamic_cast<PairValue*>(args[0].get())->getCdr();
        std::vector<ValuePtr> packedArgs;
        packedArgs.push_back(params);
        packedArgs.insert(packedArgs.end(), args.begin() + 1, args.end());
        env.defineBinding(*name, lambdaForm(packedArgs, env));
        return std::make_shared<NilValue>();
    } else {
        throw LispError("Not Implement");
    } //(define (f x y ...) a b c d ...)
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) {
        throw SyntaxError("\'if\' requires 3 arguments.");
    }
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
    return env.eval(*(args.end() - 1));
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
    //TODO: Type check
    if (args.size() < 2) {
        throw SyntaxError("\'lambda\' requires at least 2 arguments.");
    }
    std::vector<std::string> params;
    std::ranges::transform(args[0]->toVector(), std::back_inserter(params),
                           [](ValuePtr ptr) { return *ptr->asSymbol(); });
    return std::make_shared<LambdaValue>(params, std::vector<ValuePtr>(args.begin() + 1, args.end()), env.shared_from_this());
}
