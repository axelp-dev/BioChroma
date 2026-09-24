#include "TestRunner.h"
#include "Grid.h"
#include <iomanip>
#include <stdexcept>

void run_grid_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Grid & Memory Management\n";

    // Test 1 : Zero init
    {
        std::cout << "  - Default initialization to zero   " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Grid g(10, 1.0f);
        ASSERT_FLOAT_EQ(g(0, 0), 0.0f);
        ASSERT_FLOAT_EQ(g(9, 9), 0.0f);
        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2 : Set and Get
    {
        std::cout << "  - Value setting and retrieval      " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Grid g(5, 1.0f);
        g.set(2, 3, 7.5f);
        ASSERT_FLOAT_EQ(g(2, 3), 7.5f);
        ASSERT_FLOAT_EQ(g(0, 0), 0.0f);
        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3 : Exception management
    {
        std::cout << "  - Boundary conditions & exceptions " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Grid g(5, 1.0f);
        bool caught_x = false;
        bool caught_y = false;

        try { g(5, 2); } catch (const std::out_of_range&) { caught_x = true; }
        try { g.set(2, -1, 5.0f); } catch (const std::out_of_range&) { caught_y = true; }

        ASSERT_TRUE(caught_x);
        ASSERT_TRUE(caught_y);

        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4 : Deep Copy
    {
        std::cout << "  - Deep copy isolation              " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Grid original(4, 1.0f);
        original.set(1, 1, 4.0f);

        Grid copy = original; // Copy constructor
        copy.set(1, 1, 9.0f); 

        ASSERT_FLOAT_EQ(original(1, 1), 4.0f);
        ASSERT_FLOAT_EQ(copy(1, 1), 9.0f);

        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 5 : Affectation
    {
        std::cout << "  - Copy assignment operator=        " << std::setw(15) << std::setfill('.') << "";
        int current_fails = Test::failed_count;
        Grid g1(3, 1.0f);
        g1.set(0, 0, 3.0f);

        Grid g2(2, 0.5f);
        g2 = g1;

        g2.set(0, 0, 8.0f);
        ASSERT_FLOAT_EQ(g1(0, 0), 3.0f);
        ASSERT_FLOAT_EQ(g2(0, 0), 8.0f);

        if (Test::failed_count == current_fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}