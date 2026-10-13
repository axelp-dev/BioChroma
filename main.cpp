#include <string>
#include <iostream>
#include "Simulation.h"
#include "CmdParser.h"



void configure_parser(cli::Parser& parser) {
    parser.set_optional<int>("d", "duration", 50, "Simulation duration in steps."); 
    parser.set_optional<float>("t", "dt", 1.0, "Simulation step duration (dt) in seconds."); 
    parser.set_optional<bool>("v", "verbose", true, "Display simulation rendering in CLI.");
    parser.set_optional<int>("s", "seed", 0, "Random seed for all random draws to ensure reproductibility.");
    parser.set_optional<bool>("r", "real-time", false, "Erase previous display at each step.");
    parser.set_optional<float>("e", "delay", 300, "Delay between each displays;");
    // parser.set_optional<int>("o", "output", "", "Export CSV filepath for statistics");

    parser.set_required<std::string>("g", "grid_filepath", "Initialization Grid filepath."); 
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

    // Init simulation 
    SimulationConfig config = SimulationConfig(
        grid_filepath, 
        duration, 
        dt, 
        !verbose, 
        real_time, 
        delay, 
        seed, 
        ""
    ); 

    Simulation sim = Simulation(config); 
    sim.launch(); 


    return 0; 
}