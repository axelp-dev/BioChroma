#include <string>
#include <iostream>
#include "simulation/Simulation.h"
#include "io/CmdParser.h"

// TODO : Add CLI flags to stats module

void configure_parser(cli::Parser& parser) {
    // --- Main simulation parameters --- 
    // Optionnal ones
    parser.set_optional<int>("d", "duration", 50, "Simulation duration in steps."); 
    parser.set_optional<float>("t", "dt", 1.0, "Simulation step duration (dt) in seconds."); 
    parser.set_optional<bool>("v", "verbose", true, "Display simulation rendering in CLI.");
    parser.set_optional<int>("s", "seed", 0, "Random seed for all random draws to ensure reproductibility.");
    parser.set_optional<bool>("r", "real-time", false, "Erase previous display at each step.");
    parser.set_optional<float>("e", "delay", 300, "Delay between each displays;");
    
    // Required ones
    parser.set_required<std::string>("g", "grid_filepath", "Initialization Grid filepath."); 

    // --- Stats Module Parmeters --- 
    // Optionnal
    parser.set_optional<std::string>("o", "output", "data/trajectory.csv", "Export CSV filepath for statistics");
}

int main(int argc, char** argv) {

    // Init parser 
    cli::Parser parser(argc, argv);
    configure_parser(parser); 
    parser.run_and_exit_if_error();
    
    // Get arguments 
    int duration = parser.get<int>("d"); 
    float dt = parser.get<float>("t"); 
    bool verbose = parser.get<bool>("v"); 
    int seed = parser.get<int>("s"); 
    bool real_time = parser.get<bool>("r"); 
    float delay = parser.get<float>("e"); 
    std::string grid_filepath = parser.get<std::string>("g"); 
    std::string output_csv = parser.get<std::string>("o"); 

    bool generate_csv = !(output_csv == ""); 

    // Init simulation 
    SimulationConfig config = SimulationConfig(
        grid_filepath, 
        duration, 
        dt, 
        !verbose, 
        real_time, 
        delay, 
        seed, 
        output_csv, 
        generate_csv
    ); 

    Simulation sim = Simulation(config); 
    sim.launch(); 


    return 0; 
}