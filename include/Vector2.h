#ifndef VECTOR2_H
#define VECTOR2_H

// class Vector2 {
//         +float x
//         +float y
//         +Vector2()
//         +Vector2(float x, float y)
//         +Vector2(const Vector2& other)
//         +operator=(const Vector2& other) Vector2&
//         +operator+(const Vector2& other) const Vector2
//         +operator*(float scalar) const Vector2
//     }

class Vector2 {

    /*
    Implements 2d vectors mathematics calculus for simple 
    positions variations on Bacteria. 
    */
    
    public: 
        // Put coordinates into public attributes 
        // for test functions. 
        float x; 
        float y; 
        Vector2(); 
        Vector2(float x, float y); 
        Vector2(const Vector2& other); 
        Vector2& operator=(const Vector2& other); 
        Vector2 operator+(const Vector2& other) const; 
        Vector2 operator*(float scalar) const; 
}; 

#endif