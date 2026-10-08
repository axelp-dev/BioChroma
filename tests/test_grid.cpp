#include "TestRunner.h"
#include "Grid.h"
#include "Vector2.h"
#include <iomanip>
#include <stdexcept>

void run_grid_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Grid Operations & Diffusion\n";

    // Test 1 : Lecture continue via Vector2
    {
        std::cout << "  - Vector2 continuous indexing      " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(4, 1.0f);
        g.set_nutrient(2, 1, 5.5f);

        Vector2 pos(2.2f, 1.8f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(pos), 5.5f);

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

    // Test 2 : Consommation de nutriments
    {
        std::cout << "  - Nutrient consumption & underflow " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(3, 1.0f);
        g.set_nutrient(1, 1, 4.0f);

        Vector2 center(1.0f, 1.0f);
        g.consume_nutrient_at(center, 1.5f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(center), 2.5f);

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

    // Test 3 : Conservation de la masse lors de la diffusion
    {
        std::cout << "  - Mass conservation on diffusion   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(5, 1.0f);
        g.set_nutrient(2, 2, 10.0f);

        float initial_mass = 10.0f;
        g.diffuse(1.0f, 1.0f);

        float post_mass = 0.0f;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                post_mass += g.get_nutrient_at(Vector2(static_cast<float>(x), static_cast<float>(y)));
            }
        }

        ASSERT_FLOAT_EQ(initial_mass, post_mass);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(2.0f, 2.0f)), 0.0f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(2.0f, 1.0f)), 2.5f);
        ASSERT_FLOAT_EQ(g.get_nutrient_at(Vector2(1.0f, 2.0f)), 2.5f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4 : Imperméabilité des obstacles
    {
        std::cout << "  - Obstacle impermeability (-1.0)   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;
        Grid g(3, 1.0f);
        g.set_nutrient(0, 0, 4.0f);
        g.put_obstacle(1, 0);
        g.set_nutrient(0, 1, 0.0f);

        g.diffuse(1.0f, 1.0f);

        ASSERT_TRUE(g.is_obstacle(1, 0));
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