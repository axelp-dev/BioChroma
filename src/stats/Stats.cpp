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
    std::cout 
        << "Saving simulation into " 
        << this->config.export_csv << "..."
        << std::endl; 

    // Get export path 
    std::string export_path = this->config.export_csv; 
    
    // Get current date and time 
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // Build header 
    std::stringstream ss;
    ss << "# SIMULATION EXPORT - BioChroma\n"
    << "# ------------------------------\n"
    << "# Date          : " << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "\n"
    << "# Total steps   : " << this->config.total_steps << "\n"
    << "# Timepstamp    : " << this->config.dt << "\n"
    << "# Grid filepath : " << this->config.grid_filepath << "\n"
    << "# Seed          : " << this->config.seed << "\n"
    << "# ------------------------------\n\n"
    << "id,parent_id,birth_step,death_step,max_energy\n";

    std::string header = ss.str();

    // --- Build Queue Content --- 
    // String container
    std::stringstream cc; 
    
    // Unpack the Queue
    while (this->queue->empty() != true) {
        // Get front element 
        BacteriumRecord r = this->queue->front(); 
        // Cast it to string 
        cc << r.id << "," << r.parent_id << "," 
            << r.birth_step << ","
            << r.death_step << ","
            << r.max_energy_reached 
            << "\n";
        // Pop added element 
        this->queue->pop(); 
    }

    // Get string content 
    std::string content = cc.str(); 
    
    // --- CSV Writing ---
    std::ofstream file(export_path);
    if (file.is_open()) {
        // Wriging header 
        file << header;
        // Writing content 
        file << content; 
        file.close();
    } else {
        throw std::runtime_error("Unable to open export file: " + export_path);
    }
    std::cout << "Succesfully saved simulation !" << std::endl; 
}