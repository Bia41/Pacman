#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "OpenGL.hpp"
#include <cmath>
#include <vector>

#include "Vector.hpp"
#include "Eyes.hpp"
#include "Eyebrows.hpp"
#include "Maze.hpp"
#include "ParticleManager.hpp"
#include "Utilities.hpp"

#define PACMAN_RADIUS 0.6

// Velocidade a que o pacman anda
#define PACMAN_SPEED 7.0

// Declaracao dos fantasmas para impedir caso de dependencia circular (pacman conhece fantasma para o assustar e fantasma conhece pacman para olhar na sua direccao)
class Ghost;

class Pacman
{
public:
    Pacman(Vector position, Maze *maze, bool isFacingUp);
    ~Pacman(void);
    
    Vector position(void) const;
    int score(void) const;
    int lives(void) const;
    bool is_facing_up(void) const;
    Vector direction(void) const;
    bool is_dead(void) const;
    
    void set_position(Vector position);
    void set_speed(float speed);
    void set_is_facing_up(bool isFacingUp);

    void Display(void) const;
    void Update(float deltaSeconds);
    
    // Funcao que regista um fantasma para a classe do pacman passar a ter conhecimento dele
    static void RegisterGhost(Ghost *ghost);

    // Fantasmas que a classe do pacman tem registados
    static std::vector<Ghost *> ghosts;
    
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void MoveDown(float speed);
    void MoveUp(float speed);
    
    void Die(void);

private:
    Eyes *eyes_;
    Eyebrows *eyebrows_;
    
    int score_;
    int lives_;
    
    ParticleManager *particle_manager_;
    
    Vector position_;
    Vector initial_position_;
    float distance_traveled_;
    float speed_;
    Vector direction_;
    Vector wanted_direction_;
    Maze *maze_;
    
    // Angulo para usar ao desenhar as duas metades do pacman e que representa a abertura da sua boca
    float munching_angle_;
    
    // Flag que indica se o pacman esta a comer (ele so come quando anda)
    bool is_munching_;
    
    bool is_dead_;

    bool is_facing_up_;

    // Funcao que assusta os fantasmas registados
    void ScareGhosts(void) const;
};

inline Vector Pacman::position(void) const
{
    return position_;
}

inline int Pacman::score(void) const
{
    return score_;
}

inline int Pacman::lives(void) const
{
    return lives_;
}

inline bool Pacman::is_dead(void) const
{
    return is_dead_;
}

inline void Pacman::set_position(Vector position)
{
    position_ = position;
}

inline bool Pacman::is_facing_up(void) const
{
    return is_facing_up_;
}

inline void Pacman::set_is_facing_up(bool isFacingUp)
{
    is_facing_up_ = isFacingUp;
}

inline void Pacman::set_speed(float speed)
{
    speed_ = speed;
}

inline Vector Pacman::direction(void) const
{
    return direction_;
}

#endif
