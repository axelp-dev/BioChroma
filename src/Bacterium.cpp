#include "Bacterium.h"

Bacterium::Bacterium() {
    /*
    Instanciate a Bacterium object with basic 
    attributes. 
    */

    this->position = Vector2(0.0f, 0.0f); 
    this->state = BacteriumState::ALIVE; 
    this->energy = 5; 
    this->velocity = 0.3; 
    this->mutation_prob = 0.5; 
    this->id = 0; 
}

Bacterium::Bacterium(const Bacterium& other) {
    this->position = other.position; 
    this->state = other.state; 
    this->energy = other.energy; 
    this->velocity = other.velocity; 
    this->mutation_prob = other.mutation_prob; 
    this->id = other.id; 
}

Bacterium::Bacterium(Vector2 pos, float energy, float vel, float mut_prob, uint32_t id) {
    this->position = pos; 
    this->state = BacteriumState::ALIVE;  
    this->energy = energy; 
    this->velocity = vel; 
    this->mutation_prob = mut_prob;
    this->id = id; 
}

Bacterium::~Bacterium() {}

void Bacterium::update(float dt, float local_nutrient) {
    // Update Bacterium state for the simulation
    
    // 1. Random walk on the PetriDish grid
    // update the current position without considering PetriDish 
    // grid bounds : error are corriged by the Simulation()
    // ONLY FOR NON-SPORE BACTERIUM

    // Instanciate random generator 
    static thread_local std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist01(0.0f, 1.0f);
    static std::uniform_real_distribution<float> dist_angle(0.0f, 2.0f * 3.14159265f);
        
    if (this->state != BacteriumState::SPORE) {
        // Draw random argument
        float theta = dist_angle(gen);
        this->position.x += this->velocity * dt * cos(theta); 
        this->position.y += this->velocity * dt * sin(theta); 

        // Simply reduce energy with constant function for the precedant move
        this->energy -= 0.5;    
    }
    
    // 2. Nutrient update & energy consumption 
    // Get nutrients from the grid 
    this->energy += local_nutrient; 

    // 3. Change state 
    // Draw a random number 
    float u = dist01(gen);
    // Process decision values 
    float alpha = 1 - this->energy / 10; 
    float psi = 0; 
    float sigma = 0; 
    if (this->energy <= 5) {
        psi = 0; 
    } else {
        psi = 0.8 * std::pow(((this->energy - 5) / 5),2); 
    }
    if (this->energy < 2) {
        sigma = 0.5 * this->energy / 2; 
    } else if (this->energy <= 5) {
        sigma = 0.5 * (5 - this->energy) / 3; 
    } else {
        sigma = 0; 
    }
    
    if (u <= psi) {
        // Division : only change BacteriumState. The divison process 
        // is called by Simulation
        this->state = BacteriumState::DIVISION;
    } else if (u <= psi + alpha) {
        // Death
        this->state = BacteriumState::DEAD; 
    }

    // TODO : implement SPORE state and deletion of too old cells
}

Bacterium* Bacterium::clone() {
    // Process division on a Bacterium by reinitilizing 
    // the current object and creating a new one

    // 1. Create a new Bacterium 
    Bacterium* new_bac = new Bacterium(
        this->position, 
        this->energy / 2, 
        this->velocity, 
        this->mutation_prob, 
        this->id
    ); 

    // 2. Change current values 
    this->energy = this->energy / 2; 

    return new_bac; 
}
