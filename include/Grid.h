#ifndef GRID_H 
#define GRID_H

class Grid {
    
    /*
    Store PetriDish nutrients grid and implements 
    grid modifications. 
    */
    private: 
        int N; 
        float cell_size; 
        float* nutrients; 
    
    public: 
        Grid(int n , float cell_sz); 
        Grid(const Grid& other); 
        Grid& operator=(const Grid& other); 
        float operator()(int x, int y) const; 
        void set(int x, int y, float value); 
        ~Grid(); 
}; 


#endif 