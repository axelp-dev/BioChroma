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

    // Test 1 : Erreur sur fichier inexistant
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

    // Test 2 : Parsing valide depuis un fichier temporaire
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

        // Cleaning temporary file
        std::remove(tmp_filepath.c_str());

        // Population attendue : 2 bactéries
        ASSERT_TRUE(sim.get_population() == 2);

        // Validation de la grille extraite
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

    // Test 3 : Redimensionnement dynamique de la capacité (max_bacteria = 20 -> 40)
    {
        std::cout << "  - Dynamic capacity expansion       " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Grid g(6, 1.0f);
        Simulation sim(g, 0.1f);

        // Insérer 25 bactéries pour forcer le doublement de capacité au-delà de 20
        for (int i = 0; i < 25; ++i) {
            Bacterium b(Vector2(1.0f, 1.0f), 5.0f, 0.2f, 0.1f, static_cast<uint32_t>(i));
            sim.add_bacteria(&b);
        }

        ASSERT_TRUE(sim.get_population() == 25);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}