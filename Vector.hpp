#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

// Classe que representa um vector no sentido matematico, mas tambem pode ser usada para representar cores, por ex.
class Vector
{
public:
    Vector(void);
    Vector(float x, float y, float z);
    Vector(const Vector& vector);
    
    float x(void) const;
    void set_x(float x);
    float y(void) const;
    void set_y(float y);
    float z(void) const;
    void set_z(float z);
    
    Vector& operator=(const Vector& vector);
    const Vector operator+(const Vector& vector) const;
    const Vector operator+(void) const;
    Vector& operator+=(const Vector& vector);
    const Vector operator-(const Vector& vector) const;
    const Vector operator-(void) const;
    Vector& operator-=(const Vector& vector);
    const Vector operator*(float value) const;
    Vector& operator*=(float value);	
    const Vector operator/(float value) const; 
    Vector& operator/=(float value);
    
    bool operator==(const Vector& vector) const;
    bool operator!=(const Vector& vector) const;
    
    float Length(void) const;
    void Normalize(void);
    const Vector Normalized(void) const;
    float DotProduct(const Vector& vector) const;
    const Vector CrossProduct(const Vector& vector) const;
    float AngleBetweenVector(const Vector& vector) const;
    const Vector Rotated(float angle) const;
	float Distance(Vector position);

private:
    float x_;
    float y_;
    float z_;
};

inline float Vector::x(void) const
{
    return x_;
}

inline void Vector::set_x(float x)
{
    x_ = x;
}

inline float Vector::y(void) const
{
    return y_;
}

inline void Vector::set_y(float y)
{
    y_ = y;
}

inline float Vector::z(void) const
{
    return z_;
}

inline void Vector::set_z(float z)
{
    z_ = z;
}

#endif
