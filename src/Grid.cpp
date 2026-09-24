#include "Grid.h"
#include <stdexcept>
#include <cmath>

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

// Destructor 
Grid::~Grid() {
    // Only delete the nutrients array 
    delete[] this->nutrients; 
}

void Grid::set(int x, int y, float value) {
    if (x < 0 || x >= this->N || y < 0 || y >= this->N) {
        throw std::out_of_range("Coordinates out of range for Grid::set().");
    }
    this->nutrients[y * this->N + x] = value;
}

// Return nutrient value at a Vector2 point of the Grid
float Grid::get_nutrient_at(Vector2 pos) const {
    // Extract Vector2 coordinates and get cell
    int x = std::floor(pos.x); 
    int y = std::floor(pos.y);
    // Check bounds 
    if (x < 0 || x >= this->N || y < 0 || y >= this->N) {
        throw std::out_of_range("Vector2 coordinates out of range for Grid::get()."); 
    }
    return this->nutrients[y * this->N + x]; 
}

// Reduce the amount of nutrient at a specific position 
void Grid::consume_nutrient_at(Vector2 pos, float amount) {
    // Extract Vector2 coordinates and get cell
    int x = std::floor(pos.x); 
    int y = std::floor(pos.y);
    // Check bounds 
    if (x < 0 || x >= this->N || y < 0 || y >= this->N) {
        throw std::out_of_range("Vector2 coordinates out of range for Grid::consume()."); 
    } if (this->nutrients[y * this->N + x] < amount) {
        throw std::invalid_argument("Not enough nutrients to consume."); 
    }
    // Decrease nutrient amount 
    this->nutrients[y * this->N + x] -= amount; 
}


void Grid::diffuse(float dt, float diffusion_coeff) {
    (void)dt;
    float alpha = 0.25f * diffusion_coeff;
    int n = this->N;

    float* new_nutrients = new float[n * n];

    // Helper for the obstacle cases
    auto get_val = [this, n](int x, int y, float center_val) {
        float val = this->nutrients[y * n + x];
        return (val == -1.0f) ? center_val : val;
    };

    // Grid core
    for (int y = 1; y < n - 1; ++y) {
        for (int x = 1; x < n - 1; ++x) {
            int idx = y * n + x;
            float current = this->nutrients[idx];

            if (current == -1.0f) {
                new_nutrients[idx] = -1.0f;
                continue;
            }

            float left = get_val(x - 1, y, current);
            float right = get_val(x + 1, y, current);
            float top = get_val(x, y - 1, current);
            float bottom = get_val(x, y + 1, current);

            new_nutrients[idx] = current + alpha * ((left + right + top + bottom) - 4.0f * current);
        }
    }

    // Process bounds
    for (int x = 1; x < n - 1; ++x) {
        // Upper (y = 0)
        {
            int idx = 0 * n + x;
            float current = this->nutrients[idx];
            if (current == -1.0f) {
                new_nutrients[idx] = -1.0f;
            } else {
                float left = get_val(x - 1, 0, current);
                float right = get_val(x + 1, 0, current);
                float bottom = get_val(x, 1, current);
                float top = current;
                new_nutrients[idx] = current + alpha * ((left + right + top + bottom) - 4.0f * current);
            }
        }
        // Lower (y = n - 1)
        {
            int idx = (n - 1) * n + x;
            float current = this->nutrients[idx];
            if (current == -1.0f) {
                new_nutrients[idx] = -1.0f;
            } else {
                float left = get_val(x - 1, n - 1, current);
                float right = get_val(x + 1, n - 1, current);
                float top = get_val(x, n - 2, current);
                float bottom = current;
                new_nutrients[idx] = current + alpha * ((left + right + top + bottom) - 4.0f * current);
            }
        }
    }

    for (int y = 1; y < n - 1; ++y) {
        // Left (x = 0)
        {
            int idx = y * n + 0;
            float current = this->nutrients[idx];
            if (current == -1.0f) {
                new_nutrients[idx] = -1.0f;
            } else {
                float right = get_val(1, y, current);
                float top = get_val(0, y - 1, current);
                float bottom = get_val(0, y + 1, current);
                float left = current;
                new_nutrients[idx] = current + alpha * ((left + right + top + bottom) - 4.0f * current);
            }
        }
        // Right (x = n - 1)
        {
            int idx = y * n + (n - 1);
            float current = this->nutrients[idx];
            if (current == -1.0f) {
                new_nutrients[idx] = -1.0f;
            } else {
                float left = get_val(n - 2, y, current);
                float top = get_val(n - 1, y - 1, current);
                float bottom = get_val(n - 1, y + 1, current);
                float right = current;
                new_nutrients[idx] = current + alpha * ((left + right + top + bottom) - 4.0f * current);
            }
        }
    }

    // Corners
    auto process_corner = [&](int cx, int cy, int vx, int vy) {
        int idx = cy * n + cx;
        float current = this->nutrients[idx];
        if (current == -1.0f) {
            new_nutrients[idx] = -1.0f;
            return;
        }
        float neighbor_x = get_val(vx, cy, current);
        float neighbor_y = get_val(cx, vy, current);
        new_nutrients[idx] = current + alpha * ((neighbor_x + neighbor_y + current + current) - 4.0f * current);
    };

    process_corner(0, 0, 1, 1);                 // Upper-left
    process_corner(n - 1, 0, n - 2, 1);         // Upper-right
    process_corner(0, n - 1, 1, n - 2);         // Lower-left
    process_corner(n - 1, n - 1, n - 2, n - 2); // Lower-right

    // Update array
    delete[] this->nutrients;
    this->nutrients = new_nutrients;
}