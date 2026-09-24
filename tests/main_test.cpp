#include "TestRunner.h"

// Déclaration de la suite
void run_vector2_tests();
void run_grid_tests();

int main() {
    std::cout << Test::BOLD << "\n================ Running BioChroma Tests ================\n\n" << Test::RESET;

    run_vector2_tests();
    run_grid_tests(); 


    std::cout << "\n---------------------------------------------------------\n";
    int total = Test::passed_count + Test::failed_count;
    std::cout << "Results: "
              << Test::GREEN << Test::passed_count << " passed" << Test::RESET << ", "
              << (Test::failed_count > 0 ? Test::RED : "") << Test::failed_count << " failed" << Test::RESET
              << " (" << total << " total)\n\n";

    return (Test::failed_count == 0) ? 0 : 1;
}