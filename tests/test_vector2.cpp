#include "TestRunner.h"
#include "Vector2.h"
#include <iomanip>

void run_vector2_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Vector2 Algebra\n";

    // Test 1 : Default constructor
    {
        std::cout << "  - Default initialization to (0, 0) " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Vector2 v;
        ASSERT_FLOAT_EQ(v.x, 0.0f);
        ASSERT_FLOAT_EQ(v.y, 0.0f);
        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2 : Vectorial addition
    {
        std::cout << "  - Vector addition operator+        " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Vector2 a(1.5f, 2.0f);
        Vector2 b(0.5f, -1.0f);
        Vector2 c = a + b;
        ASSERT_FLOAT_EQ(c.x, 2.0f);
        ASSERT_FLOAT_EQ(c.y, 1.0f);
        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3 : Scalar multiplication
    {
        std::cout << "  - Scalar multiplication operator*  " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Vector2 a(3.0f, -4.0f);
        Vector2 b = a * 2.5f;
        ASSERT_FLOAT_EQ(b.x, 7.5f);
        ASSERT_FLOAT_EQ(b.y, -10.0f);
        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}