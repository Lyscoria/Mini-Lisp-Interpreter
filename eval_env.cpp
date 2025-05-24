#include "./eval_env.h"
#include "./forms.h"

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    }
    if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    }
    if (auto name = expr->asSymbol()) {
        auto it = symbolTable.find(*name);
        if (it != symbolTable.end()) {
            return it->second;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    }
    if (expr->isPair()) {
        auto pairExpr = dynamic_cast<PairValue*>(expr.get());
        auto exprVector = pairExpr->toVector();
        if (exprVector[0]->asSymbol()) {
            auto name = exprVector[0]->asSymbol();
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
                return SPECIAL_FORMS[*name](std::vector(exprVector.begin() + 1, exprVector.end()), *this);
            } else {
                ValuePtr proc = this->eval(exprVector[0]);
                std::vector<ValuePtr> args =
                    evalList(dynamic_cast<PairValue*>(expr.get())->getCdr());
                return this->apply(proc, args);
            }
        }
    }
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    std::ranges::transform(dynamic_cast<PairValue*>(expr.get())->toVector(), 
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return dynamic_cast<BuiltinProcValue*>(proc.get())->call(args);
    } else {
        throw LispError("Unimplemented");
    }
}

void EvalEnv::add(std::string name, ValuePtr arg) {
    symbolTable[name] = arg;
}
