#ifndef GRID_H 
#define GRID_H

#include "Vector2.h"

class Grid {
    
    /*
    Store PetriDish nutrients grid and implements 
    grid modifications. 
    Grid could have different values : 
    - between 0 and 9 -> nutrient amount
    - -1 -> obstacle
    */

    private: 
        int N; 
        float cell_size; 
        float* nutrients; 
    
    public: 
        Grid(int n , float cell_sz); 
        Grid(const Grid& other); 
        Grid& operator=(const Grid& other); 
        ~Grid(); 
        void set(int x, int y, float value); 
        void diffuse(float dt, float diffusion_coeff); 
        float get_nutrient_at(Vector2 pos) const; 
        void consume_nutrient_at(Vector2 pos, float amount); 
}; 


#endif 