#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <iostream>
#include <string>
#include <cmath>


namespace Test {
    /*
    Define namespace object Test which contains 
    all informations for a current test process. 
    
    */
    inline int passed_count = 0;
    inline int failed_count = 0;

    // ANSI code for terminal display
    constexpr const char* RESET  = "\033[0m";
    constexpr const char* GREEN  = "\033[32m\033[1m";
    constexpr const char* RED    = "\033[31m\033[1m";
    constexpr const char* CYAN   = "\033[36m\033[1m";
    constexpr const char* BOLD   = "\033[1m";
    
    // Check condition and update local counted values. 
    inline bool check_condition(bool condition, const std::string& expr, const char* file, int line) {
        if (condition) {
            ++passed_count;
            return true;
        }
        ++failed_count;
        std::cout << "\n    " << RED << "FAILED: " << RESET << expr
                  << " (" << file << ":" << line << ")";
        return false;
    }

    // Check numerical equality with an epsilon error interval
    inline bool approx_equal(float a, float b, float eps = 1e-5f) {
        return std::fabs(a - b) <= eps;
    }
}

// Define global functions to a more light usage
#define ASSERT_TRUE(expr) Test::check_condition((expr), #expr, __FILE__, __LINE__)
#define ASSERT_FLOAT_EQ(a, b) Test::check_condition(Test::approx_equal((a), (b)), #a " == " #b, __FILE__, __LINE__)

#endif