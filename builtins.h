#ifndef BUILTINS_H
#define BUILTINS_H

#include ".\value.h"
#include ".\error.h"
#include <unordered_map>
#include <string>

ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr multiply(const std::vector<ValuePtr>& params);
ValuePtr minus(const std::vector<ValuePtr>& params);
ValuePtr divide(const std::vector<ValuePtr>& params);
ValuePtr abs_new(const std::vector<ValuePtr>& params);
ValuePtr expt(const std::vector<ValuePtr>& params);
ValuePtr quotient(const std::vector<ValuePtr>& params);
ValuePtr remainder_new(const std::vector<ValuePtr>& params);
ValuePtr modulo(const std::vector<ValuePtr>& params);


ValuePtr equal(const std::vector<ValuePtr>& params);
ValuePtr less_than(const std::vector<ValuePtr>& params);
ValuePtr greater_than(const std::vector<ValuePtr>& params);
ValuePtr less_than_or_equal(const std::vector<ValuePtr>& params);
ValuePtr greater_than_or_equal(const std::vector<ValuePtr>& params);
ValuePtr even(const std::vector<ValuePtr>& params);
ValuePtr odd(const std::vector<ValuePtr>& params);
ValuePtr zero(const std::vector<ValuePtr>& params);

ValuePtr print(const std::vector<ValuePtr>& params);

extern std::unordered_map<std::string, BuiltinFuncType*> builtinFuncList;

#endif
