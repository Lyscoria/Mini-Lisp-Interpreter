#include "./eval_env.h"

ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (expr->isSelfEvaluating()) {
        return expr;
    }
    if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    }
    using namespace std::literals;
    std::vector<ValuePtr> v = expr->toVector();
    if (v[0]->asSymbol() == "define"s) {
        if (auto name = v[1]->asSymbol()) {
            symbolTable[*name] = eval(v[2]);
            return std::make_shared<NilValue>();
        } else {
            throw LispError("Malformed define.");
        }
    }
    if (auto name = expr->asSymbol()) {
        auto it = symbolTable.find(*name);
        if (it != symbolTable.end()) {
            return it->second;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    }
}
