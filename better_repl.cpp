#include "./better_repl.h"

std::string REPL::highlight(const std::string& text) {
    bool tempCommentState = false;
    auto tokens = Tokenizer::tokenize(text, tempCommentState);
    std::ostringstream result;
    int pos = 0;
    for (const auto& token : tokens) {
        const std::string& originalText = token->getOriginalText();
        int tokenPos = text.find(originalText, pos);
        if (tokenPos > pos) {
            result << text.substr(pos, tokenPos - pos);
        }
        if (token->getType() == TokenType::COMMENT) {
            result << colorize(originalText, GREEN);
        } else if (originalText.substr(0, 2) == "#|" ||
                   (originalText.find("#|") != std::string::npos &&
                    originalText.find("|#") == std::string::npos)) {
            result << colorize(originalText, GREEN);
        } else {
            result << colorizeToken(token.get());
        }
        pos = tokenPos + originalText.length();
    }
    if (pos < text.length()) {
        result << text.substr(pos);
    }
    return result.str();
}

std::string REPL::colorizeToken(Token* token) {
    const std::string& originalText = token->getOriginalText();
    if (token->getType() == TokenType::COMMENT) {
        return colorize(originalText, GREEN);
    } else if (auto str = dynamic_cast<StringLiteralToken*>(token)) {
        return colorize(originalText, LIGHT_GREEN);
    } else if (auto num = dynamic_cast<NumericLiteralToken*>(token)) {
        return colorize(originalText, LIGHT_BLUE);
    } else if (auto bool_token = dynamic_cast<BooleanLiteralToken*>(token)) {
        return colorize(originalText, LIGHT_RED);
    } else if (auto id = dynamic_cast<IdentifierToken*>(token)) {
        std::string name = id->getName();
        auto color = LIGHT_YELLOW;
        if (BUILTIN_PROCEDURES.count(name)) {
            color = YELLOW;
        }
        else if (SPECIAL_FORMS.count(name) || name == "else") {
            color = LIGHT_PURPLE;
        }
        return colorize(originalText, color);
    } else {
        switch (token->getType()) {
            case TokenType::LEFT_PAREN:
            case TokenType::RIGHT_PAREN:
            case TokenType::QUOTE:
            case TokenType::QUASIQUOTE:
            case TokenType::UNQUOTE: return colorize(originalText, LIGHT_CYAN);
            default: return colorize(originalText, YELLOW);
        }
    }
}

std::string REPL::colorize(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

void REPL::REPLMode(std::shared_ptr<EvalEnv>& env) {
    int bracketLevel = 0;
    bool inComment = false;
    std::string allInput;
    while (true) {
        try {
            if (bracketLevel < 0) {
                throw LispError("Incorrect paren.");
            }
            std::string prompt =
                (bracketLevel == 0 && !inComment) ? ">>> " : "... ";
            std::cout << prompt;
            for (int i = 0; i < bracketLevel; i++) {
                std::cout << "  ";
            }
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit") break;
            if (line.empty()) continue;
            std::cout << "\033[1A\033[K";
            std::cout << CYAN + prompt + RESET;
            for (int i = 0; i < bracketLevel; i++) {
                std::cout << "  ";
            }
            if (inComment) {
                std::cout << colorize(line, GREEN) << std::endl;
            } else {
                std::cout << highlight(line) << std::endl;
            }
            allInput += line + "\n";
            auto tokens = Tokenizer::tokenize(line, inComment);
            for (const auto& token : tokens) {
                if (token->getType() == TokenType::LEFT_PAREN) bracketLevel++;
                if (token->getType() == TokenType::RIGHT_PAREN) bracketLevel--;
            }
            if (!inComment && bracketLevel == 0 && !allInput.empty()) {
                bool tempState = false;
                auto parseTokens = Tokenizer::tokenize(allInput, tempState);
                std::deque<TokenPtr> filteredTokens;
                for (auto& token : parseTokens) {
                    if (token->getType() != TokenType::COMMENT) {
                        filteredTokens.push_back(std::move(token));
                    }
                }
                if (!filteredTokens.empty()) {
                    Parser parser(std::move(filteredTokens));
                    auto value = parser.parse();
                    auto result = env->eval(std::move(value));
                    std::cout << CYAN << result->toString() << RESET
                              << std::endl;
                }
                allInput.clear();
            }
        } catch (std::runtime_error& e) {
            std::cout << RED << "Error: " << e.what() << RESET << std::endl;
            bracketLevel = 0;
            inComment = false;
            allInput.clear();
        }
    }
}
