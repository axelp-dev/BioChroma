#include "TestRunner.h"
#include "core/Queue.h"
#include <iomanip>
#include <string>

void run_queue_tests() {
    std::cout << Test::CYAN << "[SUITE] " << Test::RESET << "Queue Container Logic\n";

    // Test 1 : Initial state and emptiness
    {
        std::cout << "  - Initial empty state check        " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Queue<int> q;
        ASSERT_TRUE(q.empty());
        ASSERT_TRUE(q.get_size() == 0);

        bool threw_on_front = false;
        try {
            q.front();
        } catch (const std::out_of_range&) {
            threw_on_front = true;
        }
        ASSERT_TRUE(threw_on_front);

        bool threw_on_pop = false;
        try {
            q.pop();
        } catch (const std::out_of_range&) {
            threw_on_pop = true;
        }
        ASSERT_TRUE(threw_on_pop);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 2 : Push, front, back, and FIFO ordering
    {
        std::cout << "  - Push and FIFO ordering           " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Queue<int> q;
        q.push(10);
        ASSERT_TRUE(!q.empty());
        ASSERT_TRUE(q.get_size() == 1);
        ASSERT_TRUE(q.front() == 10);
        ASSERT_TRUE(q.back() == 10);

        q.push(20);
        q.push(30);
        ASSERT_TRUE(q.get_size() == 3);
        ASSERT_TRUE(q.front() == 10);
        ASSERT_TRUE(q.back() == 30);

        q.pop();
        ASSERT_TRUE(q.front() == 20);
        ASSERT_TRUE(q.get_size() == 2);

        q.pop();
        ASSERT_TRUE(q.front() == 30);
        ASSERT_TRUE(q.back() == 30);

        q.pop();
        ASSERT_TRUE(q.empty());
        ASSERT_TRUE(q.get_size() == 0);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 3 : Deep copy semantics
    {
        std::cout << "  - Copy constructor and operator=   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Queue<std::string> q1;
        q1.push("alpha");
        q1.push("beta");

        Queue<std::string> q2(q1);
        ASSERT_TRUE(q2 == q1);

        q2.push("gamma");
        ASSERT_TRUE(q1.get_size() == 2);
        ASSERT_TRUE(q2.get_size() == 3);
        ASSERT_TRUE(q1 != q2);

        Queue<std::string> q3;
        q3 = q2;
        ASSERT_TRUE(q3 == q2);
        q3.pop();
        ASSERT_TRUE(q3.front() == "beta");
        ASSERT_TRUE(q2.front() == "alpha");

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 4 : Move semantics
    {
        std::cout << "  - Move constructor and operator=   " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Queue<int> q1;
        q1.push(1);
        q1.push(2);

        Queue<int> q2(std::move(q1));
        ASSERT_TRUE(q1.empty());
        ASSERT_TRUE(q2.get_size() == 2);
        ASSERT_TRUE(q2.front() == 1);

        Queue<int> q3;
        q3.push(99);
        q3 = std::move(q2);
        ASSERT_TRUE(q2.empty());
        ASSERT_TRUE(q3.get_size() == 2);
        ASSERT_TRUE(q3.front() == 1);
        ASSERT_TRUE(q3.back() == 2);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }

    // Test 5 : Clear and reuse
    {
        std::cout << "  - Clear and reuse                  " << std::setw(15) << std::setfill('.') << "";
        int fails = Test::failed_count;

        Queue<int> q;
        q.push(100);
        q.push(200);
        q.clear();

        ASSERT_TRUE(q.empty());
        ASSERT_TRUE(q.get_size() == 0);

        q.push(300);
        ASSERT_TRUE(q.get_size() == 1);
        ASSERT_TRUE(q.front() == 300);
        ASSERT_TRUE(q.back() == 300);

        if (Test::failed_count == fails) {
            std::cout << " " << Test::GREEN << "[OK]" << Test::RESET << "\n";
        } else {
            std::cout << " " << Test::RED << "[FAIL]" << Test::RESET << "\n";
        }
    }
}