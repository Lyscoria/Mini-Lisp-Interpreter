#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "./value.h"
#include "./error.h"
#include <unordered_map>

class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
    std::unordered_map<std::string, ValuePtr> symbolTable;
};

#endif
