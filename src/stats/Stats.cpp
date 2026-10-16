#include "stats/Stats.h"
#include "simulation/SimulationConfig.h"

Stats::Stats() {
    // Default constructor 
    // Init Queue storage 
    this->queue = new Queue<BacteriumRecord>(); 
    this->config = SimulationConfig(); 
}

Stats::Stats(const Stats& other) 
    : config(other.config) {
    if (other.queue != nullptr) {
        this->queue = new Queue<BacteriumRecord>(*(other.queue));
    } else {
        this->queue = new Queue<BacteriumRecord>();
    }
}

Stats::Stats(const SimulationConfig& config) {
    // Init Queue storage 
    this->queue = new Queue<BacteriumRecord>(); 
    // Copy simulation config for post logging
    this->config = config; 
}

Stats::~Stats() {
    // Delete Queue 
    delete this->queue; 
}

void Stats::insert(const BacteriumRecord& record) {
    // Insert Bacterium record object in queue 
    this->queue->push(record); 
}

void Stats::save() {
    std::cout << "Saving simulation..." << std::endl; 
    // TODO : Implement this method
}