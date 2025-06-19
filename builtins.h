#ifndef BUILTINS_H
#define BUILTINS_H

#include ".\value.h"
#include ".\error.h"
#include <unordered_map>
#include <string>

extern const std::unordered_map<std::string, BuiltinFuncType> BUILTIN_PROCEDURES;
extern const std::vector<std::string> BUILTIN_PROCEDURE_NAMES;

ValuePtr apply(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr eval(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr exit_lisp(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env);

ValuePtr isatom(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isboolean(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isinteger(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr islist(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isnumber(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isnull(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr ispair(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isprocedure(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isstring(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr issymbol(const std::vector<ValuePtr>& params, EvalEnv& env);

ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env);

ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr minus(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr multiply(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr abs_lisp(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr remainder_lisp(const std::vector<ValuePtr>& params, EvalEnv& env);

ValuePtr isEq(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isEqual(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr isNot(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr less_than(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr greater_than(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr less_than_or_equal(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr greater_than_or_equal(const std::vector<ValuePtr>& params,EvalEnv& env);
ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env);
ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env);

#endif
