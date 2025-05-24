#ifndef BUILTINS_H
#define BUILTINS_H

#include ".\value.h"
#include ".\error.h"
#include <unordered_map>
#include <string>

extern const std::unordered_map<std::string, BuiltinFuncType*> BUILTIN_PROCEDURES;

ValuePtr display(const std::vector<ValuePtr>& params);
ValuePtr exit_new(const std::vector<ValuePtr>& params);
ValuePtr newline(const std::vector<ValuePtr>& params);
ValuePtr print(const std::vector<ValuePtr>& params);

ValuePtr atom(const std::vector<ValuePtr>& params);
ValuePtr boolean(const std::vector<ValuePtr>& params);
ValuePtr integer(const std::vector<ValuePtr>& params);
ValuePtr list(const std::vector<ValuePtr>& params);
ValuePtr number(const std::vector<ValuePtr>& params);
ValuePtr null(const std::vector<ValuePtr>& params);
ValuePtr pair(const std::vector<ValuePtr>& params);
ValuePtr procedure(const std::vector<ValuePtr>& params);
ValuePtr string(const std::vector<ValuePtr>& params);
ValuePtr symbol(const std::vector<ValuePtr>& params);

ValuePtr car(const std::vector<ValuePtr>& params);
ValuePtr cdr(const std::vector<ValuePtr>& params);
ValuePtr cons(const std::vector<ValuePtr>& params);
ValuePtr length(const std::vector<ValuePtr>& params);
ValuePtr list_new(const std::vector<ValuePtr>& params);
ValuePtr append(const std::vector<ValuePtr>& params);

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

#endif
