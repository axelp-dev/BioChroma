#include "TestRunner.h"
#include "Grid.h"
#include "Vector2.h"
#include <iomanip>
#include <stdexcept>

void run_grid_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Grid Operations & Diffusion\n";

    // Test 1 : Coutinuous reading from Vector2
    {
        std::cout << "  - Vector2 continuous indexing      " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(4, 1.0f);
        g.set(2, 1, 5.5f);

        // (2.2, 1.8) into (2, 1) cell with std::floor
        Vector2 pos(2.2f, 1.8f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(pos), 5.5f);

        // Bad coordinates
        bool caught = false;
        try { 
            g.get_nutrient_at(Vector2(4.1f, 0.0f)); 
        } catch (const std::out_of_range&) { 
            caught = true; 
        }
        ASSERT_TRUE(caught);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2 : Nutrients 
    {
        std::cout << "  - Nutrient consumption & underflow " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(3, 1.0f);
        g.set(1, 1, 4.0f);

        Vector2 center(1.0f, 1.0f);
        g.consume_nutrient_at(center, 1.5f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(center), 2.5f);

        // Tentative de consommer plus que le stock disponible
        bool caught = false;
        try { 
            g.consume_nutrient_at(center, 3.0f); 
        } catch (const std::invalid_argument&) { 
            caught = true; 
        }
        ASSERT_TRUE(caught);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3 : Same nutrients mass after diffusion
    {
        std::cout << "  - Mass conservation on diffusion   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(5, 1.0f);
        g.set(2, 2, 10.0f);

        // Initial mass 
        float initial_mass = 10.0f;

        // Maximum avalaible diffusion rate
        g.diffuse(1.0f, 1.0f);

        // Post-diffusion calculus
        float post_mass = 0.0f;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                post_mass += g.get_nutrient_at(Vector2(static_cast<float>(x), static_cast<float>(y)));
            }
        }

        ASSERT_FLOAT_EQ(initial_mass, post_mass);

        // Check center
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(2.0f, 2.0f)), 0.0f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(2.0f, 1.0f)), 2.5f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(1.0f, 2.0f)), 2.5f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4 : Test bounds
    {
        std::cout << "  - Obstacle impermeability (-1.0)   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(3, 1.0f);
        g.set(0, 0, 4.0f);
        g.set(1, 0, -1.0f); // Create obstacle
        g.set(0, 1, 0.0f);

        g.diffuse(1.0f, 1.0f);

        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(1.0f, 0.0f)), -1.0f);

        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(0.0f, 0.0f)), 3.0f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(0.0f, 1.0f)), 1.0f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}