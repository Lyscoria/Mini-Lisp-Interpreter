#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "./value.h"
#include "./error.h"
#include "./builtins.h"
#include <unordered_map>
#include <algorithm>
#include <iterator>

class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> symbolTable;

public:
    EvalEnv() {
        for (auto proc : BUILTIN_PROCEDURES) {
            symbolTable.insert(std::make_pair(proc.first, std::make_shared<BuiltinProcValue>(proc.second)));
        }
    }
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    ValuePtr eval(ValuePtr expr);
    void add(std::string name, ValuePtr arg);
};

#endif
