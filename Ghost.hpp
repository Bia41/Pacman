#ifndef GHOST_HPP
#define GHOST_HPP

#include "OpenGL.hpp"
#include <cmath>

#include "Vector.hpp"
#include "Maze.hpp"
#include "Pacman.hpp"
#include "Eyes.hpp"
#include "Utilities.hpp"

#define GHOST_RADIUS 0.6

class Ghost
{
public:
    Ghost(Vector position, Maze *maze, Pacman *pacman, bool isFacingUp, Vector normalColor);
    ~Ghost(void);

    Vector position(void) const;
    Vector initial_position(void) const;

    bool is_facing_up(void) const;
    Vector direction(void) const;
    Vector stalking_direction(void) const;
    bool is_scared(void) const;
    
    void set_is_scared(bool isScared, float scaredTime);
    void set_speed(float speed);
    void set_position(Vector position);
    void set_is_facing_up(bool isFacingUp);
    
    void Display(void) const;
    void Update(float deltaSeconds);
	Vector GetStalkingDirection(std::vector<Vector> possiblePositions);
	Vector GetRunningDirection(std::vector<Vector> possiblePositions);
	std::vector<Vector> GetPossiblePositions(void);
	Vector CheckPosition(float angle);
    void Die(void);
    bool CheckPacmanCollision(void);
    
private:
    Vector position_;
    Vector initial_position_;
    
    // Direccao em que o fantasma se move
    Vector direction_;
    
    // Direccao em que o fantasma olha
    Vector stalking_direction_;
    
    float distance_traveled_;
    float speed_;
    
    // Cor do fantasma quando nao esta assustado
    Vector normal_color_;
    
    // Flag que indica se o fantasma esta assustado
    bool is_scared_;
    
    // Tempo que falta ate o fantasma deixar de estar assustado
    float scared_time_;
    
    float dead_time_;
    
    Maze *maze_;
    Pacman *pacman_;
    Eyes *eyes_;
    
    // Flag para saber se a camara e topdown ou nao
    bool is_facing_up_;
    
    bool is_dead_;
    
    GLuint display_list_;
    void CreateGhost(float radius, float height, int numberOfLegs) const;
};

inline Vector Ghost::position(void) const
{
    return position_;
}

inline Vector Ghost::initial_position(void) const
{
    return initial_position_;
}

inline bool Ghost::is_scared(void) const
{
    return is_scared_;
}

inline void Ghost::set_is_scared(bool isScared, float scaredTime)
{
    is_scared_ = isScared;
    scared_time_ = scaredTime;
    speed_ *= (isScared  == true) ? 2.0 / 3.0 : 1.0;
}

inline void Ghost::set_speed(float speed)
{
    speed_ = speed;
}

inline void Ghost::set_position(Vector position)
{
    position_ = position;
}

inline bool Ghost::is_facing_up(void) const
{
    return is_facing_up_;
}

inline void Ghost::set_is_facing_up(bool isFacingUp)
{
    is_facing_up_ = isFacingUp;
}

inline Vector Ghost::stalking_direction(void) const
{
    return stalking_direction_;
}

inline Vector Ghost::direction(void) const
{
    return direction_;
}

#endif
