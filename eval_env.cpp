#include "./eval_env.h"
#include "./forms.h"
#include "./value.h"

ValuePtr EvalEnv::eval(const ValuePtr& expr) {
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
        /*
        auto pairExpr = dynamic_cast<PairValue*>(expr.get());
        auto name = pairExpr->getCar()->asSymbol();
        if (name != std::nullopt && SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
            return SPECIAL_FORMS[*name](pairExpr->getCdr()->toVector(), *this);
        } else {
            ValuePtr proc = this->eval(pairExpr->getCar());
            std::vector<ValuePtr> args =
                evalList(expr->getCdr());
            return this->apply(proc, args);
        }
        */
        auto list = expr->toVector();
        while (list[0]->isPair()) {
            list[0] = eval(list[0]);
        }
        auto name = list[0]->asSymbol();
        if (name != std::nullopt && SPECIAL_FORMS.contains(*name)) {
            std::vector<ValuePtr> cdr_list(list.begin() + 1, list.end());
            return SPECIAL_FORMS.at(*name)(cdr_list, *this);
        }
        else {
            ValuePtr proc = eval(list[0]);
            std::vector<ValuePtr> args = evalList(expr->getCdr());
            return this->apply(proc, args);
        }
    }
    throw LispError("Unimplemented"); //TODO
}

std::vector<ValuePtr> EvalEnv::evalList(const ValuePtr& expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(expr->toVector(), 
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(const ValuePtr& proc, const std::vector<ValuePtr>& args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->call(args, *this);
    } else {
        return proc->apply(args);
    }
}

void EvalEnv::defineBinding(const std::string& name, const ValuePtr& arg) {
    SYMBOLS[name] = arg;
}

ValuePtr EvalEnv::lookupBinding(const std::string& name) {
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
