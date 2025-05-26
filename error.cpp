#include "./error.h"

std::string ArgNumberMessage(int minNum, int maxNum, int actual) {
    if (minNum == maxNum) {
        return std::format("Expected {} arguments, but received {}.", minNum, actual);
    }
    if (maxNum == -1) {
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
