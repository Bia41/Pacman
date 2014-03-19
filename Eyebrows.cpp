#include "Eyebrows.hpp"

Eyebrows::Eyebrows(void)
{
    display_list_ = glGenLists(1);
    glNewList(display_list_, GL_COMPILE);
    
    // Definir o material activo que vai ser usado para as sobrancelhas - Castanho baco
    GLfloat diffuseMaterialBrown[] = {0.5, 0.25, 0.25};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialBrown);
    GLfloat ambientMaterialBrown[] = {0.5, 0.25, 0.25};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialBrown);
    
    glPushMatrix();
    glRotatef(15.0, 0.0, 0.0, 1.0);
    glTranslatef(0.15, -0.75, -0.6);
    
    // Usa a funcao de criar uma esfera para criar uma esfera achatada em dois eixos
    Utilities::CreateSphere(0.2, 0.075, 0.075, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    glTranslatef(-0.15, -0.75, -0.6);
    Utilities::CreateSphere(0.2, 0.075, 0.075, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    glEndList();
}

Eyebrows::~Eyebrows(void)
{
    glDeleteLists(display_list_, 1);
}

void Eyebrows::Display(void) const
{
    glCallList(display_list_);
}
