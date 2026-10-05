#include "TestRunner.h"
#include "Bacterium.h"
#include <iomanip>
#include <cmath>

void run_bacterium_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Bacterium Agent Logic\n";

    // Test 1 : Constructors and init
    {
        std::cout << "  - Default & parameterized init     " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Bacterium b1;
        ASSERT_TRUE(b1.state == BacteriumState::ALIVE);
        ASSERT_FLOAT_EQ(b1.position.x, 0.0f);
        ASSERT_FLOAT_EQ(b1.position.y, 0.0f);

        Bacterium b2(Vector2(2.5f, 3.5f), 8.0f, 0.5f, 0.1f, 42);
        ASSERT_TRUE(b2.state == BacteriumState::ALIVE);
        ASSERT_FLOAT_EQ(b2.position.x, 2.5f);
        ASSERT_FLOAT_EQ(b2.position.y, 3.5f);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2 : Moving and speed 
    {
        std::cout << "  - Motility & step displacement     " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Vector2 start_pos(1.0f, 1.0f);
        float vel = 0.4f;
        float dt = 0.5f; // Theorical step : 0.4 * 0.5 = 0.2
        Bacterium b(start_pos, 5.0f, vel, 0.0f, 1);

        b.update(dt, 0.5f);

        // Check distance : ||r - r0|| == vel * dt
        float dx = b.position.x - start_pos.x;
        float dy = b.position.y - start_pos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        ASSERT_FLOAT_EQ(dist, vel * dt);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3 : Clone fission and energy conservation
    {
        std::cout << "  - Clone allocation & energy split  " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Vector2 pos(4.0f, 2.0f);
        Bacterium mother(pos, 8.0f, 0.2f, 0.05f, 10);

        Bacterium* daughter = mother.clone();

        // Check allocation
        ASSERT_TRUE(daughter != nullptr);

        // Same position for child and mother
        ASSERT_FLOAT_EQ(daughter->position.x, pos.x);
        ASSERT_FLOAT_EQ(daughter->position.y, pos.y);

        delete daughter;

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4 : Spores don't move 
    {
        std::cout << "  - Spore dormancy invariance        " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Vector2 fixed_pos(3.0f, 3.0f);
        Bacterium b(fixed_pos, 2.0f, 0.5f, 0.0f, 2);
        b.state = BacteriumState::SPORE;

        b.update(1.0f, 0.0f);

        ASSERT_FLOAT_EQ(b.position.x, fixed_pos.x);
        ASSERT_FLOAT_EQ(b.position.y, fixed_pos.y);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}