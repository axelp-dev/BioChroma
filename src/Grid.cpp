#include "Grid.h"
#include <stdexcept>

// Default constructor
Grid::Grid(int n , float cell_sz) {
    this->N = n; 
    this->cell_size = cell_sz; 
    // Init nutrients array on the HEAP
    this->nutrients = new float[this->N * this->N]; 
    // Init values to zero 
    for (int i = 0; i < this->N * this->N; i++) {
        this->nutrients[i] = 0.0f; 
    }
}

// Copy constructor 
Grid::Grid(const Grid& other) {
    this->N = other.N; 
    this->cell_size = other.cell_size; 
    // Init nutrients grid and use deepcopy 
    this->nutrients = new float[this->N * this->N]; 
    for (int i = 0; i < this->N * this->N; i++) {
        this->nutrients[i] = other.nutrients[i]; 
    }
}

// Affectation as copy constructor
Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {
        delete[] this->nutrients; 
        this->N = other.N; 
        this->cell_size = other.cell_size; 
        this->nutrients = new float[this->N * this->N];
        // Deep copy of nutrients array 
        for (int i = 0; i < this->N * this->N; i++) {
            this->nutrients[i] = other.nutrients[i]; 
        }
    }
    return *this; 
}

// Use ...() as getter for nutrient value 
float Grid::operator()(int x, int y) const {
    // Check x and y values 
    if (x >= this->N || x < 0 || y >= this->N || y < 0) {
        throw std::out_of_range("Index out of range for grid object."); 
    } 
    // Return the given value 
    return this->nutrients[y * this->N + x]; 
}

// Change a value on the grid
void Grid::set(int x, int y, float value) {
    // Check x and y values 
    if (x >= this->N || x < 0 || y >= this->N || y < 0) {
        throw std::out_of_range("Index out of range for grid object."); 
    } if (value < 0 || value > 9) {
        throw std::out_of_range("Set value out of range for grid.");
    }
    // Change the value
    this->nutrients[y * this->N + x] = value; 
}

// Deestructor 
Grid::~Grid() {
    // Only delete the nutrients array 
    delete[] this->nutrients; 
}

