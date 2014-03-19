#include "ParticleManager.hpp"

ParticleManager::ParticleManager(void)
{
    for (int i = 0; i < MAXIMUM_PARTICLES; ++i) {
        particles_[i].isActive = false;
    }
    
    is_active_ = false;
    active_particles_ = MAXIMUM_PARTICLES;
}

ParticleManager::~ParticleManager(void)
{

}

void ParticleManager::MakeBoing(Vector position)
{
    for (int i = 0; i < MAXIMUM_PARTICLES; ++i) {
        particles_[i].position = position;
        particles_[i].velocity = Vector(((rand() % 100) / 20.0 - 2.5) * ((rand() % 100) / 20.0 - 2.5),
                                        ((rand() % 100) / 20.0 - 2.5) * ((rand() % 100) / 20.0 - 2.5),
                                        (rand() % 100) / 10.0 - 10.0);
        particles_[i].isActive = true;
    }

    is_active_ = true;
}

void ParticleManager::Update(float deltaSeconds)
{
    for (int i = 0; i < MAXIMUM_PARTICLES; ++i) {
        if (particles_[i].isActive == true) {
            particles_[i].velocity += Vector(0.0, 0.0, 9.8) * deltaSeconds;
            particles_[i].position += particles_[i].velocity * deltaSeconds;
            
            if (particles_[i].position.z() > 1.0) {
                particles_[i].isActive = false;
                active_particles_--;
            }
        }
    }
    
    if (active_particles_ <= 0) {
        is_active_ = false;
    }
}

void ParticleManager::Display(void)
{
    GLfloat diffuseMaterialYellow[] = {1.0, 1.0, 0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialYellow);
    GLfloat ambientMaterialYellow[] = {1.0, 1.0, 0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialYellow);
    
    for (int i = 0; i < MAXIMUM_PARTICLES; ++i) {
        if (particles_[i].isActive == true) {
            glPushMatrix();
            glTranslatef(particles_[i].position.x(), particles_[i].position.y(), particles_[i].position.z());
            
            glutSolidSphere(0.1, 4, 4);
            
            glPopMatrix();
        }
    }

}
