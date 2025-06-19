#include <iostream>
#include <fstream>
#include <string>

#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./read.h"
#define RJSJ_TEST_NO_EXIT
#include "rjsj_test.hpp"

struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::create();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

std::shared_ptr<EvalEnv> env = EvalEnv::create();

int main(int argc, char* argv[]) {
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
    if (argc == 1) {
        REPLMode(env);
    } else {
        std::string filepath = argv[1];
        fileMode(env, filepath);
    }
}
