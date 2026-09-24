#include "Vector2.h"

// Init Vector2 at (0.0, 0.0)
Vector2::Vector2() : x(0.0f), y(0.0f) {}

// Init Vector2 with given position 
Vector2::Vector2(float x, float y) : x(x), y(y) {}

// Init Vector2 as the same position than a given one
Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) {}

// Affectation same as copy constructor
Vector2& Vector2::operator=(const Vector2& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
    }
    return *this; 
}

// Additionning two vectors components by components
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(this->x * scalar, this->y * scalar);
}

