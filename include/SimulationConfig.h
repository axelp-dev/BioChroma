#ifndef SIMULATION_CONFIG 
#define SIMULATION_CONFIG 

#include <string>


struct SimulationConfig {
    std::string grid_filepath = "data/grid-examples/example_grid10.txt"; 
    int total_steps = 50; 
    float dt = 1.0; 
    bool headless = false; 
    uint32_t seed = 0; 
    std::string export_csv = ""; 

    // Default constructor
    SimulationConfig() = default;
    // Copy constructor 
    SimulationConfig(const SimulationConfig& other) = default;
    SimulationConfig& operator=(const SimulationConfig& other) = default;

    SimulationConfig(std::string grid_filepath, int total_steps, float dt, bool headless, uint32_t seed, std::string export_csv) 
        : grid_filepath(grid_filepath), 
        total_steps(total_steps), 
        dt(dt), 
        headless(headless), 
        seed(seed), 
        export_csv(export_csv) {}; 

    
    SimulationConfig(float dt, int total_steps) {
        // Constructor used for grid initialization
        this->grid_filepath = ""; 
        this->total_steps = total_steps; 
        this->dt = dt; 
        this->headless = false; 
        this->seed = 0; 
        this->export_csv = ""; 
    }; 
}; 


#endif 