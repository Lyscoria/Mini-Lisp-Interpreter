#ifndef BUILTINS_H
#define BUILTINS_H

#include ".\value.h"
#include ".\error.h"
#include <unordered_map>
#include <string>

ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr print(const std::vector<ValuePtr>& params);

extern std::unordered_map<std::string, BuiltinFuncType*> builtinFuncList;

#endif
