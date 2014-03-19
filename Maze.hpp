#ifndef MAZE_HPP
#define MAZE_HPP

#include "OpenGL.hpp"
#include <fstream>
#include <string>
#include "Utilities.hpp"
#include "TextureManager.hpp"

// Caracteres a utilizar quando se faz um labirinto em ".txt"
enum MazeBlock {
    LEFT_BOTTOM_CORNER = 't',
    LEFT_TOP_CORNER = 'f',
    RIGHT_BOTTOM_CORNER = 'J',
    RIGHT_TOP_CORNER = '7',
    HORIZONTAL_WALL = '~',
    VERTICAL_WALL = '|',
    PELLET = '.',
    EMPTY_SPACE = ' ',
    DOOR = 'D',
    SUPER_PELLET = 'o',
    TELEPORT = 'T',
    FORBIDDEN_SPACE = 'X'
};

// Classe que representa o labirinto, o qual e constituido por tudo menos o pacman e os fantasmas
class Maze
{
public:
    Maze(const std::string fileName);
    ~Maze(void);
    
    int width(void) const;
    int height(void) const;
    char *data(void) const;
    int number_of_pellets(void) const;
    void set_number_of_pellets(int numberOfPellets);
    
    void Display(void) const;
    
private:
    int width_;
    int height_;
    char *data_;
    int number_of_pellets_;
    GLuint maze_display_list_;
    GLuint pellet_display_list_;
    GLuint super_pellet_display_list_;
    GLuint floor_texture_name_;
    void DisplayCorner(float angle, int i, int j) const;
};

inline void Maze::set_number_of_pellets(int numberOfPellets)
{
    number_of_pellets_ = numberOfPellets;
}

inline int Maze::number_of_pellets(void) const
{
    return number_of_pellets_;
}

inline int Maze::width(void) const
{
    return width_;
}

inline int Maze::height(void) const
{
    return height_;
}

inline char *Maze::data(void) const
{
    return data_;
}

#endif
