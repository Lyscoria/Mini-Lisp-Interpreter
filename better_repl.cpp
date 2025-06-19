#include "./better_repl.h"

#include "./better_repl.h"

std::string REPL::highlight(const std::string& text) {
    auto tokens = Tokenizer::tokenize(text);
    std::ostringstream result;
    size_t pos = 0;
    for (const auto& token : tokens) {
        const std::string& originalText = token->getOriginalText();
        size_t tokenPos = text.find(originalText, pos);
        if (tokenPos > pos) {
            result << text.substr(pos, tokenPos - pos);
        }
        result << colorizeToken(token.get());
        pos = tokenPos + originalText.length();
    }
    if (pos < text.length()) {
        result << text.substr(pos);
    }
    return result.str();
}

std::string REPL::colorizeToken(Token* token) {
    const std::string& originalText = token->getOriginalText();
    if (auto str = dynamic_cast<StringLiteralToken*>(token)) {
        return colorize(originalText, GREEN);
    } else if (auto num = dynamic_cast<NumericLiteralToken*>(token)) {
        return colorize(originalText, GREEN);
    } else if (auto bool_token = dynamic_cast<BooleanLiteralToken*>(token)) {
        return colorize(originalText, MAGENTA);
    } else if (auto id = dynamic_cast<IdentifierToken*>(token)) {
        std::string name = id->getName();
        auto color = (BUILTIN_PROCEDURES.count(name) ||
                      SPECIAL_FORMS.count(name) || name == "else") ? MAGENTA : YELLOW;
        return colorize(originalText, color);
    } else {
        switch (token->getType()) {
            case TokenType::LEFT_PAREN:
            case TokenType::RIGHT_PAREN: return colorize(originalText, CYAN);
            case TokenType::QUOTE:
            case TokenType::QUASIQUOTE:
            case TokenType::UNQUOTE: return colorize(originalText, MAGENTA);
            default: return colorize(originalText, YELLOW);
        }
    }
}

std::string REPL::colorize(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

void REPL::REPLMode(std::shared_ptr<EvalEnv>& env) {
    int bracketLevel = 0;
    std::string sentence;
    while (true) {
        try {
            std::string prompt = (bracketLevel == 0) ? ">>> " : "... ";
            std::cout << prompt;
            for (int i = 0; i < bracketLevel; i++) {
                std::cout << "  ";
            }
            std::string line;
            std::getline(std::cin, line);
            if (line == "exit") break;
            if (line.empty()) continue;
            std::cout << "\033[1A\033[K";
            std::cout << prompt;
            for (int i = 0; i < bracketLevel; i++) {
                std::cout << "  ";
            }
            std::cout << highlight(line) << std::endl;
            for (char c : line) {
                if (c == '(') bracketLevel++;
                if (c == ')') bracketLevel--;
            }
            sentence += " " + line;
            if (bracketLevel == 0 && !sentence.empty()) {
                auto tokens = Tokenizer::tokenize(sentence);
                Parser parser(std::move(tokens));
                auto value = parser.parse();
                auto result = env->eval(std::move(value));
                std::cout << CYAN << result->toString() << RESET << std::endl;
                sentence.clear();
            }
        } catch (std::runtime_error& e) {
            std::cout << "\033[31mError: " << e.what() << RESET << std::endl;
            bracketLevel = 0;
            sentence.clear();
        }
    }
}
