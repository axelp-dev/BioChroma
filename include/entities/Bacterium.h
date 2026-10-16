#ifndef BACTERIUM_H
#define BACTERIUM_H

#include "core/Vector2.h"
#include "core/Types.h"
#include "stats/Stats.h"
#include <stdint.h>
#include <random>
#include <math.h>
#include <cmath>

class Bacterium {

    private: 
        float energy; 
        float velocity; 
        float mutation_prob; 
        uint32_t id;
        BacteriumRecord* record; 

    
    public: 
        // Attributes 
        BacteriumState state; 
        Vector2 position; 

        // Methods
        Bacterium(); 
        Bacterium(const Bacterium& other); 
        Bacterium(Vector2 pos, float energy, float vel, float mut_prob, uint32_t id); 
        virtual ~Bacterium(); 
        virtual void update(float dt, float local_nutrient); 
        virtual Bacterium* clone(); 

        // NOTE : Implementing a death() method for Bacteria record return ? 

        // Getter/Setter
        void set_id(uint32_t id) {
            // Update local id 
            this->id = id;
            // Push local id to BacteriumRecord 
            this->record->id = id; 
        }; 
        void set_birth_step(uint32_t n_step) {
            this->record->birth_step = n_step; 
        }
}; 





#endif 