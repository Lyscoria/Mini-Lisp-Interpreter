#include <iostream>
#include <fstream>
#include <string>

#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./read.h"
#include "rjsj_test.hpp"

std::shared_ptr<EvalEnv> env = EvalEnv::create();

int main(int argc, char* argv[]) {
    if (argc == 1) {
        REPLMode(env);
    } else {
        std::string filepath = argv[1];
        fileMode(env, filepath);
    }
}
