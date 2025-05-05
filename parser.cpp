#include "./parser.h"
#include "./error.h"

ValuePtr Parser::parse() {
    if (tokens.empty()) {
        throw SyntaxError("EOF");
    }
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    if (token->getType() == TokenType::LEFT_PAREN) {
        return parseTails();
    } 
    if (token->getType() == TokenType::QUOTE) {
        return Value::toList(std::vector<ValuePtr>{
            std::make_shared<SymbolValue>("quote"), this->parse(), std::make_shared<NilValue>()});
    }
    if (token->getType() == TokenType::QUASIQUOTE) {
        return Value::toList(std::vector<ValuePtr>{
            std::make_shared<SymbolValue>("quasiquote"), this->parse(), std::make_shared<NilValue>()});
    }
    if (token->getType() == TokenType::UNQUOTE) {
        return Value::toList(std::vector<ValuePtr>{
            std::make_shared<SymbolValue>("unquote"), this->parse(), std::make_shared<NilValue>()});
    }
    throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw SyntaxError("EOF");
    }
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = this->parse();
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    } else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}
