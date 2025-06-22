#include "./read.h"
#include "./better_repl.h"

void REPLMode(std::shared_ptr<EvalEnv>& env) {
    REPL interface;
    interface.REPLMode(env);
}

void fileMode(std::shared_ptr<EvalEnv>& env, const std::string& filepath) {
    std::ifstream input{filepath};
    if (!input) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
    }
    std::string file((std::istreambuf_iterator<char>(input)),
                     std::istreambuf_iterator<char>());
    try {
        bool commentState = false;
        auto tokens = Tokenizer::tokenize(file, commentState);
        std::deque<TokenPtr> filteredTokens;
        for (auto& token : tokens) {
            if (token->getType() != TokenType::COMMENT) {
                filteredTokens.push_back(std::move(token));
            }
        }
        Parser parser(std::move(filteredTokens));
        while (!parser.end()) {
            auto value = parser.parse();
            env->eval(std::move(value));
        }
    } catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
