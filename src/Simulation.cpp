#include "Simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

Simulation::Simulation(Grid& grid, float dt) {
    this->grid = new Grid(grid); 
    this->dt = dt;
    this->current_step = 0; 
    this->n_bacteria = 0; 
    this->max_bacteria = 20; 
    this->diffusion_coeff = 0.5; 
    this->next_bacterium_id = 0; 
    // Init Bacterium array to 10 elements 
    this->bacteria = new Bacterium*[this->max_bacteria]; 
}

Simulation::Simulation(const Simulation& other) {
    this->grid = new Grid(*(other.grid)); 
    this->dt = other.dt; 
    this->current_step = other.current_step; 
    this->n_bacteria = other.n_bacteria; 
    this->max_bacteria = other.max_bacteria; 
    this->diffusion_coeff = other.diffusion_coeff;
    this->next_bacterium_id = other.next_bacterium_id; 
    // Bacteria array deep-copy 
    this->bacteria = new Bacterium*[other.max_bacteria]; 
    for (int i = 0; i < other.n_bacteria; i++) {
        this->bacteria[i] = other.bacteria[i]; 
    }
}

Simulation::Simulation(const std::string& filepath) {
    this->current_step = 0;
    this->grid = nullptr;
    this->max_bacteria = 20;
    this->n_bacteria = 0;
    this->diffusion_coeff = 0.5f;
    this->dt = 0.1f;
    this->bacteria = new Bacterium*[this->max_bacteria];
    this->next_bacterium_id = 0; 

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to load file: " + filepath);
    }

    int n_value = 0;
    float cell_size = 1.0f;

    enum class ParseSection { HEADER, NUTRIENTS, BACTERIA };
    ParseSection section = ParseSection::HEADER;

    std::string line;
    int grid_y = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Détection des délimiteurs de section
        if (line.rfind("--- NUTRIENTS", 0) == 0 || line == "---") {
            section = ParseSection::NUTRIENTS;
            grid_y = 0;
            continue;
        }
        if (line.rfind("--- BACTERIA", 0) == 0) {
            section = ParseSection::BACTERIA;
            grid_y = 0;
            continue;
        }

        // 1. Parsing de l'en-tête
        if (section == ParseSection::HEADER) {
            std::stringstream ss(line);
            std::string key;
            ss >> key;
            if (key == "N") {
                ss >> n_value;
            } else if (key == "C" || key == "CELL_SIZE") {
                ss >> cell_size;
            } else if (key == "D" || key == "DIFFUSION") {
                ss >> this->diffusion_coeff;
            }

            // Si toutes les métadonnées requises sont lues, allouer la grille
            if (n_value > 0 && !this->grid) {
                this->grid = new Grid(n_value, cell_size);
            }
            continue; // Ne pas traiter les lignes d'en-tête comme des cellules !
        }

        if (!this->grid) {
            throw std::runtime_error("Grid dimensions (N) must be defined before grid data.");
        }

        // 2. Traitement des lignes de matrices (grid_y)
        for (size_t x = 0; x < line.size() && static_cast<int>(x) < n_value; ++x) {
            char c = line[x];

            if (section == ParseSection::NUTRIENTS) {
                if (c == '#') {
                    this->grid->put_obstacle(static_cast<int>(x), grid_y);
                } else if (c >= '0' && c <= '9') {
                    this->grid->set_nutrient(static_cast<int>(x), grid_y, static_cast<float>(c - '0'));
                }
            } else if (section == ParseSection::BACTERIA) {
                if (c == 'B') {
                    if (this->grid->is_obstacle(static_cast<int>(x), grid_y)) {
                        throw std::runtime_error("Trying to put a Bacterium on a wall!");
                    }
                    Vector2 p(static_cast<float>(x), static_cast<float>(grid_y));
                    Bacterium* b = new Bacterium(p, 5.0f, 1.0f, 0.2f, next_bacterium_id++);
                    this->add_bacteria(b);
                }
            }
        }
        ++grid_y;
    }

    file.close();
}


Simulation::~Simulation() {
    // Delete Bacteria array 
    delete[] this->bacteria; 
    // Call Grid destructor
    delete this->grid; 
}

void Simulation::add_bacteria(Bacterium* b) {
    if (this->n_bacteria + 1 == this->max_bacteria) {
        // Double the Bacterium array capacity 
        int new_max = this->max_bacteria * 2; 
        Bacterium** new_array = new Bacterium*[new_max]; 
        // Deepcopy 
        for (int i = 0; i < this->n_bacteria; i++) {
            new_array[i] = this->bacteria[i]; 
        }
        // Change values 
        this->max_bacteria = new_max; 
        delete[] this->bacteria; 
        this->bacteria = new_array; 
    }
    // Add bacterium and update counter 
    this->bacteria[this->n_bacteria] = b; 
    this->n_bacteria += 1; 
}


void Simulation::display() {
    if (!this->grid) {
        std::cerr << "Error : None Grid in Simulation object.\n";
        return;
    }

    // ANSI codes
    const std::string RESET       = "\033[0m";
    const std::string BOLD        = "\033[1m";
    const std::string DIM         = "\033[2m";
    
    // Coloes
    const std::string COLOR_WALL  = "\033[38;5;240m"; 
    const std::string COLOR_BAC   = "\033[1;32m";
    const std::string COLOR_DIV   = "\033[1;33m";
    const std::string COLOR_SPORE = "\033[1;36m";

    // Nutrients palette
    const std::string NUTRIENT_COLORS[10] = {
        "\033[38;5;236m",
        "\033[38;5;24m",
        "\033[38;5;25m",
        "\033[38;5;31m",
        "\033[38;5;37m",
        "\033[38;5;43m",
        "\033[38;5;42m",
        "\033[38;5;48m",
        "\033[38;5;46m",
        "\033[38;5;82m" 
    };

    int n = this->grid->get_n(); 

    // Simulation header
    std::cout << "\n" << BOLD << "┌── BioChroma ─────────────────────────────┐" << RESET << "\n";
    std::cout << "│ Step: " << BOLD << this->current_step << RESET 
              << "\t Population: " << BOLD << this->n_bacteria << RESET << " agents\t   │\n";
    std::cout << "└" << std::string(n * 2 + 2, '-') << "┘\n";

    // Display box
    for (int y = 0; y < n; ++y) {
        std::cout << DIM << "│ " << RESET;
        for (int x = 0; x < n; ++x) {

            // Check for en agent on the cell (x, y)
            const Bacterium* found = nullptr;
            for (int i = 0; i < this->n_bacteria; ++i) {
                if (this->bacteria[i]->state != BacteriumState::DEAD) {
                    int bx = static_cast<int>(std::floor(this->bacteria[i]->position.x));
                    int by = static_cast<int>(std::floor(this->bacteria[i]->position.y));
                    if (bx == x && by == y) {
                        found = this->bacteria[i];
                        break;
                    }
                }
            }

            // Display agent as text
            if (found) {
                if (found->state == BacteriumState::DIVISION) {
                    std::cout << COLOR_DIV << "D " << RESET;
                } else if (found->state == BacteriumState::SPORE) {
                    std::cout << COLOR_SPORE << "S " << RESET;
                } else {
                    std::cout << COLOR_BAC << "B " << RESET;
                }
            } else if (this->grid->is_obstacle(x, y)) {
                std::cout << COLOR_WALL << "██" << RESET;
            } else {
                float val = this->grid->get_nutrient_at(Vector2(static_cast<float>(x), static_cast<float>(y)));
                int level = static_cast<int>(std::round(val));
                if (level < 0) level = 0;
                if (level > 9) level = 9;

                if (level == 0) {
                    std::cout << DIM << "· " << RESET;
                } else {
                    std::cout << NUTRIENT_COLORS[level] << level << " " << RESET;
                }
            }
        }
        std::cout << DIM << "│" << RESET << "\n";
    }

    std::cout << " " << std::string(n * 2 + 3, '-') << "\n";
}


void Simulation::delete_dead_bacteria() {
    // Delete all DEAD bacteria from the storage array Bacterium** bacteria
    int i = 0; 
    while (i < this->n_bacteria) {
        // Check if the bacteria is DEAD 
        if (this->bacteria[i]->state == BacteriumState::DEAD) {
            // Exchange this Bacterium with the last one 
            delete this->bacteria[i]; 
            this->bacteria[i] = this->bacteria[this->n_bacteria - 1]; 
            this->bacteria[this->n_bacteria - 1] = nullptr; 
            this->n_bacteria--; 
        } else {
            i++; 
        }
    }
}


void Simulation::step() {
    // Proceed to a Simulation step as described in the ARCHITECTURE.md file. 

    // 1. Diffuse nutrients on the Grid 
    this->grid->diffuse(this->dt, this->diffusion_coeff); 

    // 2. Update each Bacterium state 
    int initial_pop = this->n_bacteria; 
    for (int i = 0; i < initial_pop; i++) {
        // Get current Bacterium 
        Bacterium* b = this->bacteria[i]; 
        if (b->state == BacteriumState::DEAD) continue;
        
        // Update nutrients 
        float nut = this->grid->get_nutrient_at(b->position); 
        // Consume nutrients on the Grid 
        float intake = std::min(nut, 1.0f); 
        this->grid->consume_nutrient_at(b->position, intake); 
        
        // Update Bacterium 
        b->update(this->dt, nut); 

        // Wrap Bacterium coordinates of out of range 
        if (b->position.x >= this->grid->get_n()) {
            b->position.x = 0; 
        }
        if (b->position.y >= this->grid->get_n()) {
            b->position.y = 0; 
        }

        // Manage division 
        if (b->state == BacteriumState::DIVISION) {
            Bacterium* daughter = b->clone(); 
            daughter->set_id(this->next_bacterium_id); 
            this->add_bacteria(daughter); 
            this->next_bacterium_id++; 
            
            // Update mother state 
            b->state = BacteriumState::ALIVE; 
        }
    }

    // 3. Delete DEAD Bacterium 
    this->delete_dead_bacteria(); 
    this->current_step++;
}


void Simulation::launch(int total_steps) {
    // Launch simulation for a total_steps steps

    for (int i = 0; i < total_steps; i++) {
        // Update hidden state 
        this->step(); 
        // Display PetriDish
        this->display(); 
    }
}