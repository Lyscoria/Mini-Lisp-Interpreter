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
    const std::string RESET = "\033[0m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string GRAY = "\033[90m";
    const std::string CYAN = "\033[36m";

    std::string colorizeToken(Token* token);

public:
    std::string colorize(const std::string& text, const std::string& color);
    std::string highlight(const std::string& text);
    void REPLMode(std::shared_ptr<EvalEnv>& env);
};

#endif
