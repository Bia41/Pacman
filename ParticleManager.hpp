#ifndef PARTICLE_MANAGER_HPP
#define PARTICLE_MANAGER_HPP

#include <cstdlib>
#include "OpenGL.hpp"
#include "Particle.hpp"
#include "Vector.hpp"
#include "Utilities.hpp"

#define MAXIMUM_PARTICLES 500

class ParticleManager
{
public:
    ParticleManager(void);
    ~ParticleManager(void);
    
    bool is_active(void) const;
    int active_particles() const;
    void set_active_particles(int activeParticles);
    
    void Update(float deltaSeconds);
    void Display(void);
    void MakeBoing(Vector position);
    
private:
    Particle particles_[MAXIMUM_PARTICLES];
    bool is_active_;
    int active_particles_;
};

inline bool ParticleManager::is_active() const
{
    return is_active_;
}

inline int ParticleManager::active_particles() const
{
    return active_particles_;
}

inline void ParticleManager::set_active_particles(int activeParticles)
{
    active_particles_ = activeParticles;
}

#endif
