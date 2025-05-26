#include "./eval_env.h"
#include "./forms.h"
#include "./value.h"

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    }
    if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    }
    if (auto name = expr->asSymbol()) {
        return lookupBinding(*name);
    }
    if (expr->isPair()) {
        auto pairExpr = dynamic_cast<PairValue*>(expr.get());
        auto exprVector = pairExpr->toVector();
        auto name = exprVector[0]->asSymbol();
        if (name != std::nullopt && SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
            return SPECIAL_FORMS[*name](pairExpr->getCdr()->toVector(), *this);
        } else {
            ValuePtr proc = this->eval(exprVector[0]);
            std::vector<ValuePtr> args =
                evalList(dynamic_cast<PairValue*>(expr.get())->getCdr());
            return this->apply(proc, args);
        }
    }
    throw LispError("Unimplemented"); //TODO
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(), 
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return dynamic_cast<BuiltinProcValue*>(proc.get())->call(args, *this);
    } else {
        return dynamic_cast<LambdaValue*>(proc.get())->apply(args);
    }
}

void EvalEnv::defineBinding(std::string name, ValuePtr arg) {
    SYMBOLS[name] = arg;
}

ValuePtr EvalEnv::lookupBinding(std::string name) {
    auto it = SYMBOLS.find(name);
    if (it != SYMBOLS.end()) {
        return it->second;
    } else {
        if (parent == nullptr) {
            throw LispError("Variable " + name + " not defined.");
        }
        return parent->lookupBinding(name);
    }
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, 
    const std::vector<ValuePtr>& args) {
    std::unordered_map<std::string, ValuePtr> childSYMBOLS;
    if (params.size() != args.size()) {
        throw LispError("Params not pair with args.");
    }
    for (int i = 0; i < params.size(); i++) {
        childSYMBOLS[params[i]] = args[i];
    }
    return EvalEnv::create(childSYMBOLS, get_shared_this());
}
