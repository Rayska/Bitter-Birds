#include <iostream>
#include <stdexcept>
#include <string>

#include "test_utils.hpp"
#include "reader_writer_test.hpp"

void dummytest1() {
    verify(1+1==2, "no");
}

int main() {
    try {
        dummytest1();
        tests_reader_writer();

        std::cout << "All tests passed" << std::endl;
    } catch (const TestAssertionException& e) {
        std::cerr << "Test assertion failed" << e.what() << std::endl;
    }


    return 0;
}