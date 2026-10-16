#ifndef STATS_H
#define STATS_H

#include "core/Queue.h"
#include "simulation/SimulationConfig.h"
#include "string"
#include <iostream>

// Define Bacterium record container for Queue elements 
struct BacteriumRecord {
    uint32_t id;
    uint32_t parent_id;
    uint32_t birth_step;
    uint32_t death_step;
    float max_energy_reached;

    // Constructor 
    // NOTE : Only need id, parent id and birth_step at instanciation 
    // because death_step and max_energy_reached is updating at Queue insertion
    // when Bacterium deads. 
    BacteriumRecord(uint32_t id, uint32_t parent_id, uint32_t birth_step) 
        : id(id), parent_id(parent_id), birth_step(birth_step) {
            this->death_step = 0; 
            this->max_energy_reached = -1; 
        }; 
    // Copy constructor
    BacteriumRecord(const BacteriumRecord& other) 
        : id(other.id), parent_id(other.parent_id), birth_step(other.birth_step), 
            death_step(other.death_step), max_energy_reached(other.max_energy_reached)
        {}; 
}; 

class Stats {

    private: 
        // Queue Bacterium record 
        Queue<BacteriumRecord>* queue; 
        // Output folder filepath 
        std::string output_folder; 
        // SimulationConfig object for post-logging 
        SimulationConfig config; 
    
    public: 
        // Constructors
        Stats(); 
        Stats(const SimulationConfig& config, std::string output); 
        ~Stats(); 

        // I/O
        void insert(const BacteriumRecord& record); 
        void save();
}; 






#endif 