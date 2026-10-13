#include "TestRunner.h"
#include "SimulationConfig.h"
#include "Simulation.h"
#include "Grid.h"
#include <iomanip>
#include <fstream>
#include <cstdio>

void run_simulation_config_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "SimulationConfig Initialization & Injection\n";

    // Test 1: Default constructor initial values
    {
        std::cout << "  - Default values check             " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        SimulationConfig cfg;

        ASSERT_TRUE(cfg.grid_filepath == "data/grid-examples/example_grid10.txt");
        ASSERT_TRUE(cfg.total_steps == 50);
        ASSERT_FLOAT_EQ(cfg.dt, 1.0f);
        ASSERT_TRUE(!cfg.headless);
        ASSERT_TRUE(cfg.seed == 0);
        ASSERT_TRUE(cfg.export_csv.empty());

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2: Parameterized and partial constructors
    {
        std::cout << "  - Custom & partial constructors    " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        // Full parameter constructor
        SimulationConfig full_cfg("custom_path.txt", 120, 0.05f, true, 42, "out.csv");
        ASSERT_TRUE(full_cfg.grid_filepath == "custom_path.txt");
        ASSERT_TRUE(full_cfg.total_steps == 120);
        ASSERT_FLOAT_EQ(full_cfg.dt, 0.05f);
        ASSERT_TRUE(full_cfg.headless);
        ASSERT_TRUE(full_cfg.seed == 42);
        ASSERT_TRUE(full_cfg.export_csv == "out.csv");

        // Partial constructor used for manual grid injection
        SimulationConfig partial_cfg(0.25f, 200);
        ASSERT_TRUE(partial_cfg.grid_filepath.empty());
        ASSERT_TRUE(partial_cfg.total_steps == 200);
        ASSERT_FLOAT_EQ(partial_cfg.dt, 0.25f);
        ASSERT_TRUE(!partial_cfg.headless);
        ASSERT_TRUE(partial_cfg.seed == 0);
        ASSERT_TRUE(partial_cfg.export_csv.empty());

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3: Copy semantics
    {
        std::cout << "  - Copy constructor integrity       " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        SimulationConfig source("source.txt", 80, 0.02f, true, 1337, "log.csv");
        SimulationConfig copy(source);

        ASSERT_TRUE(copy.grid_filepath == source.grid_filepath);
        ASSERT_TRUE(copy.total_steps == source.total_steps);
        ASSERT_FLOAT_EQ(copy.dt, source.dt);
        ASSERT_TRUE(copy.headless == source.headless);
        ASSERT_TRUE(copy.seed == source.seed);
        ASSERT_TRUE(copy.export_csv == source.export_csv);

        // Mutate source to verify value independence
        source.total_steps = 999;
        source.grid_filepath = "modified.txt";
        ASSERT_TRUE(copy.total_steps == 80);
        ASSERT_TRUE(copy.grid_filepath == "source.txt");

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4: Injection into Simulation instances
    {
        std::cout << "  - Simulation injection & copy ctor " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        const std::string tmp_file = "test_cfg_tmp.txt";
        {
            std::ofstream out(tmp_file);
            out << "N 2\n"
                << "CELL_SIZE 1.0\n"
                << "DIFFUSION 0.4\n"
                << "--- NUTRIENTS ---\n"
                << "00\n"
                << "00\n"
                << "--- BACTERIA ---\n"
                << "..\n"
                << "..\n";
        }

        SimulationConfig cfg(tmp_file, 75, 0.04f, true, 99, "metrics.csv");
        Simulation sim(cfg);
        std::remove(tmp_file.c_str());

        // Verify config reached the Simulation instance
        ASSERT_TRUE(sim.get_config().total_steps == 75);
        ASSERT_FLOAT_EQ(sim.get_config().dt, 0.04f);
        ASSERT_TRUE(sim.get_config().headless);

        // Verify deep copy preserves internal config
        Simulation sim_copy(sim);
        ASSERT_TRUE(sim_copy.get_config().total_steps == 75);
        ASSERT_FLOAT_EQ(sim_copy.get_config().dt, 0.04f);
        ASSERT_TRUE(sim_copy.get_config().grid_filepath == tmp_file);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}