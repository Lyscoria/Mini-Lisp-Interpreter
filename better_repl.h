#ifndef BETTER_REPL_H
#define BETTER_REPL_H

#include <iostream>
#include <sstream>
#include <string>

#include "./builtins.h"
#include "./eval_env.h"
#include "./forms.h"
#include "./parser.h"
#include "./tokenizer.h"

class REPL {
private:
    const std::string RESET = "\033[m";
    const std::string RED = "\033[0;32;31m";
    const std::string LIGHT_RED = "\033[1;31m";
    const std::string GREEN = "\033[0;32;32m";
    const std::string LIGHT_GREEN = "\033[1;32m";
    const std::string BLUE = "\033[0;32;34m";
    const std::string LIGHT_BLUE = "\033[1;34m";
    const std::string DARK_GRAY = "\033[1;30m";
    const std::string CYAN = "\033[0;36m";
    const std::string LIGHT_CYAN = "\033[1;36m";
    const std::string PURPLE = "\033[0;35m";
    const std::string LIGHT_PURPLE = "\033[1;35m";
    const std::string BROWN = "\033[0;33m";
    const std::string LIGHT_YELLOW = "\033[1;33m";
    const std::string YELLOW = "\033[0;33m";
    const std::string LIGHT_GRAY = "\033[0;37m";
    const std::string WHITE = "\033[1;37m";

    std::string colorizeToken(Token* token);

public:
    std::string colorize(const std::string& text, const std::string& color);
    std::string highlight(const std::string& text);
    void REPLMode(std::shared_ptr<EvalEnv>& env);
};

#endif
