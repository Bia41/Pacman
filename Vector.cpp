#include "Vector.hpp"

Vector::Vector(void)
{
    
}

Vector::Vector(float x, float y, float z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

Vector::Vector(const Vector& vector)
{
    x_ = vector.x_;
    y_ = vector.y_;
    z_ = vector.z_;
}

Vector& Vector::operator=(const Vector& vector)
{
    x_ = vector.x_;
    y_ = vector.y_;
    z_ = vector.z_;
    return *this;
}

const Vector Vector::operator+(const Vector& vector) const
{
    return Vector(x_ + vector.x_, y_ + vector.y_, z_ + vector.z_);
}

const Vector Vector::operator+(void) const
{
    return Vector(x_, y_, z_);
}

Vector& Vector::operator+=(const Vector& vector)
{
    x_ += vector.x_;
    y_ += vector.y_;
    z_ += vector.z_;
    return *this;
}

const Vector Vector::operator-(const Vector& vector) const
{
    return Vector(x_ - vector.x_, y_ - vector.y_, z_ - vector.z_);
}

const Vector Vector::operator-(void) const
{
    return Vector(-x_, -y_, -z_);
}

Vector& Vector::operator-=(const Vector& vector)
{
    x_ -= vector.x_;
    y_ -= vector.y_;
    z_ -= vector.z_;
    return *this;
}

const Vector Vector::operator*(float value) const
{
    return Vector(x_ * value, y_ * value, z_ * value);
}

Vector& Vector::operator*=(float value)
{
    x_ *= value;
    y_ *= value;
    z_ *= value;
    return *this;
}

const Vector Vector::operator/(float value) const
{
    return Vector(x_ / value, y_ / value, z_ / value);
}

Vector& Vector::operator/=(float value)
{
    x_ /= value;
    y_ /= value;
    z_ /= value;
    return *this;
}

bool Vector::operator==(const Vector& vector) const
{
    return ((x_ == vector.x_) && (y_ == vector.y_) && (z_ == vector.z_));
}

bool Vector::operator!=(const Vector& vector) const
{
    return ((x_ != vector.x_) || (y_ != vector.y_) || (z_ != vector.z_));
}

// Tamanho do vector
float Vector::Length(void) const
{
    return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
} 

// Normaliza o vector - torna-o de tamanho unitario
void Vector::Normalize(void)
{
    float length = std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
    x_ /= length;
    y_ /= length;
    z_ /= length;
}

// Retorna um vector normalizado - nao altera o vector utilizado
const Vector Vector::Normalized(void) const
{
    float length = std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
    return Vector(x_ / length, y_ / length, z_ / length);
}

// Calcula o produto interno
float Vector::DotProduct(const Vector& vector) const
{
    return ((x_ * vector.x_) + (y_ * vector.y_) + (z_ * vector.z_));
}

// Calcula o produto externo
const Vector Vector::CrossProduct(const Vector& vector) const
{
    return Vector(y_ * vector.z_ - vector.y_ * z_, vector.x_ * z_ - x_ * vector.z_, x_ * vector.y_ - vector.x_ * y_);  
}

// Calcula o angulo entre dois vectores - nao esquecer que o valor que retorna pode representar dois angulos diferentes pois e usado o acos
float Vector::AngleBetweenVector(const Vector& vector) const
{
    return acos(this->DotProduct(vector) / (this->Length() * vector.Length())) * (180.0 / M_PI);
}

// Retorna um vector rodado um determinado numero de graus sobre o eixo dos zz
const Vector Vector::Rotated(float angle) const
{
    float angleInRadians = angle * (M_PI / 180.0);
    return Vector(x_ * cos(angleInRadians) - y_ * sin(angleInRadians), x_ * sin(angleInRadians) + y_ * cos(angleInRadians), 0.0);
}

float Vector::Distance(Vector position)
{
	return sqrt(pow((position.x() - x_), 2) + pow((position.y() - y_), 2));
}
