#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>
#include <exception>
#include "./value.h"
#include <format>
#include <string>

class LispError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class SyntaxError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

std::string ArgNumberMessage(int minNum, int maxNum, int actual);

class ArgNumberError : public std::runtime_error {
public:
    ArgNumberError(int minNum, int maxNum, int actual)
        : runtime_error(ArgNumberMessage(minNum, maxNum, actual)) {}
};

std::string ArgTypeMessage(std::string expected, std::string actual);

class ArgTypeError : public std::runtime_error {
public:
    ArgTypeError(std::string expected, std::string actual)
        : runtime_error(ArgTypeMessage(expected, actual)) {}
};

class SignalError : public std::exception {
private:
    ValuePtr value;

public:
    SignalError(ValuePtr value) : value{value} {};
    const char* what() const noexcept override {
        return value->toString().c_str();
    }
};

void checkArgNumber(int min, int max, const std::vector<ValuePtr>& params);

void checkArgType(const std::vector<std::string> types,
                  const std::vector<ValuePtr>& params);

#endif
