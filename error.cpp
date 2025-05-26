#include "./error.h"

std::string ArgNumberMessage(int minNum, int maxNum, int actual) {
    if (minNum == maxNum) {
        return std::format("Expected {} arguments, but received {}.", minNum, actual);
    }
    if (maxNum == INT_MAX) {
        return std::format("Expected at least {} arguments, but received {}.", minNum, actual);
    }
    if (minNum == -1) {
        return std::format("Expected no more than {} arguments, but received {}.", maxNum, actual);
    }
    return std::format("Expected {} to {} arguments, but received {}.", minNum, maxNum, actual);
}

std::string ArgTypeMessage(std::string expected, std::string actual) {
    return std::format("Expected {}, but received {}", expected, actual);
}

void checkArgNumber(int min, int max, const std::vector<ValuePtr>& params) {
    if (params.size() < min || params.size() > max) {
        throw ArgNumberError(min, max, params.size());
    }
    return;
}

void checkArgType(const std::vector<std::string> types,
                  const std::vector<ValuePtr>& params) {
    for (int idx = 0; idx < types.size(); idx++) {
        if (params[idx]->getType() != types[idx]) {
            throw ArgTypeError(types[idx], params[idx]->getType());
        }
    }
    return;
}
