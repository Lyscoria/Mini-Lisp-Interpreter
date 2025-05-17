#include "./eval_env.h"
#include <algorithm>
#include <iterator>
using namespace std::literals;

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
        auto ptr = dynamic_cast<PairValue*>(expr.get());
        std::vector<ValuePtr> v = ptr->toVector();
        if (v[0]->asSymbol() == "define"s) {
            if (auto name = v[1]->asSymbol()) {
                symbolTable[*name] = eval(v[2]);
                return std::make_shared<NilValue>();
            } else {
                throw LispError("Malformed define.");
            }
        }
        ValuePtr proc = this->eval(v[0]);
        std::vector<ValuePtr> args =
            evalList(dynamic_cast<PairValue*>(expr.get())->getRight());
        return this->apply(proc, args);
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
