#ifndef SIMULATION_H
#define SIMULATION_H

#include "Bacterium.h"
#include "Grid.h"
#include <string>

class Simulation {
    
    private: 
        Bacterium* bacteria; 
        int n_bacteria; 
        int max_bacteria; 
        Grid* grid; 
        float dt; 
        float diffusion_coeff; 
        uint32_t current_step;

    public: 
        Simulation(Grid& grid, float dt); 
        Simulation(const Simulation& other); 
        Simulation(const std::string& filepath); 
        ~Simulation(); 
        void add_bacteria(const Bacterium* b); 
        void delete_bacteria(); 
        void step(); 
        void launch(uint32_t total_steps); 
        void display(); 
        // friend class Logger
}; 






#endif 