#include "Eyes.hpp"

Eyes::Eyes(void)
{
    display_list_ = glGenLists(1);
    glNewList(display_list_, GL_COMPILE);
    
    // Valores para o material que vai ser usado para as iris - Preto baco
    GLfloat diffuseMaterialBlack[] = {0.0, 0.0, 0.0};
    GLfloat ambientMaterialBlack[] = {0.0, 0.0, 0.0};
    
    // Valores para o material que vai ser usado para os olhos - Branco baco
    GLfloat diffuseMaterialWhite[] = {1.0, 1.0, 1.0};
    GLfloat ambientMaterialWhite[] = {1.0, 1.0, 1.0};
    
    glPushMatrix();
    
    // Material de cor preta da iris do olho esquerdo
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialBlack);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialBlack);
    
    glTranslatef(-0.2, 0.1, -0.1);	
    
    glPushMatrix();
    glTranslatef(0.0, -0.25, -0.55);
    Utilities::CreateSphere(0.1, 0.2, 0.1, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    // Material de cor branca do olho esquerdo
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialWhite);
    
    glPushMatrix();
    glTranslatef(0.0, -0.3, -0.5);
    Utilities::CreateSphere(0.175, 0.4, 0.1, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    glPopMatrix();
    
    glPushMatrix();
    
    // Material de cor preta da iris do olho direito
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialBlack);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialBlack);
    
    glTranslatef(0.2, 0.1, -0.1);
    
    glPushMatrix();
    glTranslatef(0.0, -0.25, -0.55);
    Utilities::CreateSphere(0.1, 0.2, 0.1, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    // Material de cor branca do olho direito
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialWhite);
    
    glPushMatrix();
    glTranslatef(0.0, -0.3, -0.5);
    Utilities::CreateSphere(0.175, 0.4, 0.1, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glPopMatrix();
    
    glPopMatrix();
    
    glEndList();
}

Eyes::~Eyes(void)
{
    glDeleteLists(display_list_, 1);
}

void Eyes::Display(void) const
{
    glCallList(display_list_);
}
