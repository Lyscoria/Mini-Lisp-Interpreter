#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "./value.h"
#include "./error.h"
#include "./builtins.h"
#include <unordered_map>

class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;

public:
    EvalEnv() {
        for (auto x : builtinFuncList) {
            symbolTable.insert(std::make_pair(x.first, std::make_shared<BuiltinProcValue>(x.second)));
        }
    }
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
};

#endif
