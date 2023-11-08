#include <iostream>
#include <stdexcept>
#include <string>

class TestAssertionException : public std::exception {
public:
    TestAssertionException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};


void verify(bool condition, const std::string& message) {
    if (!condition) {
        throw TestAssertionException(message);
    }
}