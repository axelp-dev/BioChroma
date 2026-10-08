#ifndef BACTERIUM_H
#define BACTERIUM_H

#include "Vector2.h"
#include "Types.h"
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
}; 





#endif 