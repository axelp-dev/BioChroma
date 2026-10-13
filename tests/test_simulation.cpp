#include "TestRunner.h"
#include "Simulation.h"
#include "Grid.h"
#include "Vector2.h"
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <cstdio>

void run_simulation_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Simulation Engine & Config Parsing\n";

    // Test 1: Exception thrown on missing scenario file
    {
        std::cout << "  - Missing file exception           " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        bool caught = false;
        try {
            Simulation sim("non_existent_file_path_xyz.txt");
        } catch (const std::runtime_error&) {
            caught = true;
        }
        ASSERT_TRUE(caught);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2: Valid ASCII scenario parsing from temporary file
    {
        std::cout << "  - ASCII scenario parsing           " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        const std::string tmp_filepath = "test_scenario_tmp.txt";
        {
            std::ofstream out(tmp_filepath);
            out << "N 4\n"
                << "CELL_SIZE 1.0\n"
                << "DIFFUSION 0.6\n"
                << "--- NUTRIENTS ---\n"
                << "####\n"
                << "#50#\n"
                << "#00#\n"
                << "####\n"
                << "--- BACTERIA ---\n"
                << "....\n"
                << ".B..\n"
                << "..B.\n"
                << "....\n";
        }

        Simulation sim(tmp_filepath);

        // Clean up temporary file
        std::remove(tmp_filepath.c_str());

        // Expected population: 2 bacteria
        ASSERT_TRUE(sim.get_population() == 2);

        // Validate parsed grid layout
        const Grid* g = sim.get_grid();
        ASSERT_TRUE(g != nullptr);
        ASSERT_TRUE(g->is_obstacle(0, 0));
        ASSERT_TRUE(g->is_obstacle(3, 3));
        ASSERT_FLOAT_EQ(g->get_nutrient_at(Vector2(1.0f, 1.0f)), 5.0f);
        ASSERT_FLOAT_EQ(g->get_nutrient_at(Vector2(2.0f, 1.0f)), 0.0f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3: Dynamic capacity expansion beyond initial threshold (20 -> 40)
    {
        std::cout << "  - Dynamic capacity expansion       " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Grid g(6, 1.0f);
        Simulation sim(g, 0.1f);

        // Add 25 heap-allocated bacteria to trigger capacity doubling beyond 20
        for (int i = 0; i < 25; ++i) {
            Bacterium* b = new Bacterium(Vector2(1.0f, 1.0f), 5.0f, 0.2f, 0.1f, static_cast<uint32_t>(i));
            sim.add_bacteria(b);
        }

        ASSERT_TRUE(sim.get_population() == 25);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}