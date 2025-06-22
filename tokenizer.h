#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <deque>
#include <string>

#include "./token.h"

class Tokenizer {
private:
    TokenPtr nextToken(int& pos);
    std::deque<TokenPtr> tokenize();

    std::string input;
    bool& inComment;

    Tokenizer(const std::string& input, bool& commentState)
        : input{input}, inComment{commentState} {}

public:
    static std::deque<TokenPtr> tokenize(const std::string& input);
    static std::deque<TokenPtr> tokenize(const std::string& input,
                                         bool& commentState);
};

#endif
