#include <iostream>
#include <fstream>
#include <string>

#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"
#include "./eval_env.h"
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
        while (true) {
            try {
                std::cout << ">>> ";
                std::string line;
                std::getline(std::cin, line);
                if (std::cin.eof()) {
                    std::exit(0);
                }
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                auto value = parser.parse();
                auto result = env->eval(std::move(value));
                std::cout << result->toString() << std::endl;
            } catch (std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        std::string filepath = argv[1];
        std::ifstream input{filepath};
        if (!input) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
        }
        std::string file((std::istreambuf_iterator<char>(input)),
                            std::istreambuf_iterator<char>());
        try {
            auto tokens = Tokenizer::tokenize(file);
            Parser parser(std::move(tokens));
            while (!parser.end()) {
                auto value = parser.parse();
                env->eval(std::move(value));
            }
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
