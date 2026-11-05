#include "TestRunner.h"
#include "stats/Stats.h"
#include "simulation/SimulationConfig.h"
#include <iomanip>

void run_stats_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Stats Module & Records\n";

    // Test 1: Record construction and default values
    {
        std::cout << "  - BacteriumRecord initialization   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        BacteriumRecord rec(1, 0, 10);
        ASSERT_TRUE(rec.id == 1);
        ASSERT_TRUE(rec.parent_id == 0);
        ASSERT_TRUE(rec.birth_step == 10);
        ASSERT_TRUE(rec.death_step == 0);
        ASSERT_FLOAT_EQ(rec.max_energy_reached, -1.0f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2: Stats insertion without crash
    {
        std::cout << "  - Stats insertion lifecycle        " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        SimulationConfig cfg;
        Stats stats(cfg);

        BacteriumRecord rec1(10, 0, 0);
        rec1.death_step = 45;
        rec1.max_energy_reached = 12.5f;

        stats.insert(rec1);

        BacteriumRecord rec2(11, 10, 20);
        rec2.death_step = 50;
        rec2.max_energy_reached = 8.0f;

        stats.insert(rec2);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}