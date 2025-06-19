#ifndef READ_H
#define READ_H
#include <iostream>
#include <fstream>
#include <string>
#include "./parser.h"
#include "./tokenizer.h"
#include "./eval_env.h"

void REPLMode(std::shared_ptr<EvalEnv>& env);
void fileMode(std::shared_ptr<EvalEnv>& env, const std::string& filepath);

#endif
