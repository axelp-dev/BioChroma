#ifndef SIMULATION_H
#define SIMULATION_H

#include "entities/Bacterium.h"
#include "simulation/Grid.h"
#include "simulation/SimulationConfig.h"
#include <string>

class Simulation {
    
    private: 
        SimulationConfig config; 
        Bacterium** bacteria; 
        int n_bacteria; 
        int max_bacteria; 
        Grid* grid; 
        float diffusion_coeff; 
        uint32_t current_step;
        uint32_t next_bacterium_id;

    public: 
        Simulation(Grid& grid, float dt, int total_steps = 50); 
        Simulation(const Simulation& other); 
        Simulation(const SimulationConfig& config); 
        Simulation(const std::string& filepath); 
        ~Simulation(); 
        void add_bacteria(Bacterium* b); 
        void delete_dead_bacteria(); 
        void step(); 
        Vector2 wrap_position(const Vector2& pos, int N); 
        void launch(); 
        void display(); 

        // Accessors for testing and inspection
        int get_population() const { return this->n_bacteria; }
        const Grid* get_grid() const { return this->grid; }
        const SimulationConfig& get_config() const { return this->config; }
}; 

#endif