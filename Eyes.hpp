#ifndef EYES_HPP
#define EYES_HPP

#include "OpenGL.hpp"
#include "Utilities.hpp"

// Classe que define um par de olhos
class Eyes
{
public: 
    Eyes(void);
    ~Eyes(void);
    
    void Display(void) const;
    
private:
    GLuint display_list_;
};

#endif
