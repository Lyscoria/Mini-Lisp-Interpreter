#include "./tokenizer.h"

#include <cctype>
#include <set>
#include <stdexcept>

#include "./error.h"

const std::set<char> TOKEN_END{'(', ')', '\'', '`', ',', '"'};

TokenPtr Tokenizer::nextToken(int& pos) {
    while (pos < input.size()) {
        auto c = input[pos];
        if (c == ';') {
            while (pos < input.size() && input[pos] != '\n') {
                pos++;
            }
        } else if (std::isspace(c)) {
            pos++;
        } else if (auto token = Token::fromChar(c)) {
            token->setOriginalText(std::string(1, c));
            pos++;
            return token;
        } else if (c == '#') {
            if (auto result = BooleanLiteralToken::fromChar(input[pos + 1])) {
                result->setOriginalText(input.substr(pos, 2));
                pos += 2;
                return result;
            } else {
                throw SyntaxError("Unexpected character after #");
            }
        } else if (c == '"') {
            int start = pos;
            std::string string;
            pos++;
            while (pos < input.size()) {
                if (input[pos] == '"') {
                    pos++;
                    auto token = std::make_unique<StringLiteralToken>(string);
                    token->setOriginalText(input.substr(start, pos - start));
                    return token;
                } else if (input[pos] == '\\') {
                    if (pos + 1 >= input.size()) {
                        throw SyntaxError("Unexpected end of string literal");
                    }
                    auto next = input[pos + 1];
                    if (next == 'n') {
                        string += '\n';
                    } else {
                        string += next;
                    }
                    pos += 2;
                } else {
                    string += input[pos];
                    pos++;
                }
            }
            throw SyntaxError("Unexpected end of string literal");
        } else {
            int start = pos;
            do {
                pos++;
            } while (pos < input.size() && !std::isspace(input[pos]) &&
                     !TOKEN_END.contains(input[pos]));
            auto text = input.substr(start, pos - start);
            TokenPtr token;
            if (text == ".") {
                token = Token::dot();
            } else if (std::isdigit(text[0]) || text[0] == '+' || text[0] == '-' || text[0] == '.') {
                try {
                    token = std::make_unique<NumericLiteralToken>(std::stod(text));
                } catch (std::invalid_argument& e) {
                    token = std::make_unique<IdentifierToken>(text);
                }
            } else {
                token = std::make_unique<IdentifierToken>(text);
            }
            token->setOriginalText(text);
            return token;
        }
    }
    return nullptr;
}

std::deque<TokenPtr> Tokenizer::tokenize() {
    std::deque<TokenPtr> tokens;
    int pos = 0;
    while (true) {
        auto token = nextToken(pos);
        if (!token) {
            break;
        }
        tokens.push_back(std::move(token));
    }
    return tokens;
}

std::deque<TokenPtr> Tokenizer::tokenize(const std::string& input) {
    return Tokenizer(input).tokenize();
}
