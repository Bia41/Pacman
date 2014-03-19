#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"

typedef struct particle
{
    Vector position;
    Vector velocity;
    bool isActive;
} Particle;

#endif
