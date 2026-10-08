#include "Simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>

Simulation::Simulation(Grid& grid, float dt) {
    this->grid = &grid; 
    this->dt = dt;
    this->current_step = 0; 
    this->n_bacteria = 0; 
    this->max_bacteria = 20; 
    this->diffusion_coeff = 0.5; 
    // Init Bacterium array to 10 elements 
    this->bacteria = new Bacterium[this->max_bacteria]; 
}

Simulation::Simulation(const Simulation& other) {
    this->grid = other.grid; 
    this->dt = other.dt; 
    this->current_step = other.current_step; 
    this->n_bacteria = other.n_bacteria; 
    this->max_bacteria = other.max_bacteria; 
    this->diffusion_coeff = 0.5; 
    // Bacteria array deep-copy 
    this->bacteria = new Bacterium[other.n_bacteria]; 
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
    this->bacteria = new Bacterium[this->max_bacteria];

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to load file: " + filepath);
    }

    int n_value = 0;
    float cell_size = 1.0f;
    uint32_t bac_id = 0;

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
                    Bacterium b(p, 5.0f, 1.0f, 0.2f, bac_id++);
                    this->add_bacteria(&b);
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

void Simulation::add_bacteria(const Bacterium* b) {
    if (this->n_bacteria + 1 == this->max_bacteria) {
        // Double the Bactrium array capacity 
        int new_max = this->max_bacteria * 2; 
        Bacterium* new_array = new Bacterium[new_max]; 
        // Deepcopy 
        for (int i = 0; i < this->max_bacteria; i++) {
            new_array[i] = this->bacteria[i]; 
        }
        // Change values 
        this->max_bacteria = new_max; 
        delete[] this->bacteria; 
        this->bacteria = new_array; 
    }
    // Add bacterium and update counter 
    this->bacteria[this->n_bacteria] = *b; 
    this->n_bacteria += 1; 
}


void Simulation::display() {
    if (!this->grid) {
        std::cerr << "Erreur : aucune grille initialisee.\n";
        return;
    }

    // Codes d'échappement ANSI
    const std::string RESET       = "\033[0m";
    const std::string BOLD        = "\033[1m";
    const std::string DIM         = "\033[2m";
    
    // Couleurs agents & décor
    const std::string COLOR_WALL  = "\033[38;5;240m"; // Gris sombre
    const std::string COLOR_BAC   = "\033[1;32m";     // Vert vif (ALIVE)
    const std::string COLOR_DIV   = "\033[1;33m";     // Jaune or (DIVISION)
    const std::string COLOR_SPORE = "\033[1;36m";     // Cyan (SPORE)

    // Palette nutriments (du bleu profond au vert lumineux)
    const std::string NUTRIENT_COLORS[10] = {
        "\033[38;5;236m", // 0 : quasi invisible
        "\033[38;5;24m",  // 1 : bleu très sombre
        "\033[38;5;25m",  // 2 : bleu foncé
        "\033[38;5;31m",  // 3 : bleu moyen
        "\033[38;5;37m",  // 4 : cyan-vert sombre
        "\033[38;5;43m",  // 5 : cyan-vert clair
        "\033[38;5;42m",  // 6 : vert moyen
        "\033[38;5;48m",  // 7 : vert menthe
        "\033[38;5;46m",  // 8 : vert brillant
        "\033[38;5;82m"   // 9 : vert saturé éclatant
    };

    int n = this->grid->get_n(); 

    // En-tête de simulation
    std::cout << "\n" << BOLD << "┌── BioChroma ─────────────────────────────┐" << RESET << "\n";
    std::cout << "│ Step: " << BOLD << this->current_step << RESET 
              << "\t Population: " << BOLD << this->n_bacteria << RESET << " agents\t   │\n";
    std::cout << "└" << std::string(n * 2 + 2, '-') << "┘\n";

    // Affichage de la boîte
    for (int y = 0; y < n; ++y) {
        std::cout << DIM << "│ " << RESET;
        for (int x = 0; x < n; ++x) {

            // 1. Recherche d'un agent sur la cellule (x, y)
            const Bacterium* found = nullptr;
            for (int i = 0; i < this->n_bacteria; ++i) {
                if (this->bacteria[i].state != BacteriumState::DEAD) {
                    int bx = static_cast<int>(std::floor(this->bacteria[i].position.x));
                    int by = static_cast<int>(std::floor(this->bacteria[i].position.y));
                    if (bx == x && by == y) {
                        found = &this->bacteria[i];
                        break;
                    }
                }
            }

            // 2. Rendu contextuel
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