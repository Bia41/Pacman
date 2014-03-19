#ifndef EYEBROWS_HPP
#define EYEBROWS_HPP

#include "OpenGL.hpp"
#include "Utilities.hpp"

// Um objecto da classe Eyebrows define duas sobrancelhas
class Eyebrows
{
public: 
    Eyebrows(void);
    ~Eyebrows(void);
    
    void Display(void) const;
    
private:
    GLuint display_list_;
};

#endif
