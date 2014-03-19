#include "Pacman.hpp"

// Incluir o ficheiro header do fantasma por causa da dependencia circular
#include "Ghost.hpp"

// Velocidade com que o pacman vai abrindo e fechando a boca
static const float MUNCHING_SPEED = 3.0;

// Tempo que os fantasmas ficam assustados
static const int GHOSTS_SCARED_TIME = 10.0;

// Valor da pontuacao das bolas pequenas
static const int PELLET_POINTS = 2;

// Definicao da variavel de classe que guarda os fantasmas registados
std::vector<Ghost *> Pacman::ghosts;

Pacman::Pacman(Vector position, Maze *maze, bool isFacingUp)
{
    position_ = position;
    initial_position_ = position;
    
    direction_ = Vector(0.0, 1.0, 0.0);
    wanted_direction_ = Vector(0.0, 1.0, 0.0);
    
    munching_angle_ = 0.0;
    is_munching_ = false;
    is_dead_ = false;
    
    score_ = 0;
    lives_ = 3;
    particle_manager_ = 0;
    
    speed_ = 0.0;
    distance_traveled_ = 0.0;
    
    eyes_ = new Eyes();
    eyebrows_ = new Eyebrows();
    
    is_facing_up_ = isFacingUp;   
    particle_manager_ = new ParticleManager();
    
    maze_ = maze;
}

void Pacman::Die()
{
    lives_--;
    is_dead_ = true;
    particle_manager_->MakeBoing(position_);
    for (unsigned int i = 0; i < ghosts.size(); ++i) {
        // Coloca os fantasmas assustados com tempo de GHOSTS_SCARED_TIME segundos
        Pacman::ghosts[i]->set_position(Pacman::ghosts[i]->initial_position());
    }
}

Pacman::~Pacman(void)
{
    delete eyebrows_;
    delete eyes_;
    delete particle_manager_;
}

// O pacman sao duas meias esferas juntas
void Pacman::Display(void) const
{
    if (is_dead_ == false) {
        glPushMatrix();
        
        glTranslatef(position_.x(), position_.y(), position_.z());
        
        glRotatef(direction_.AngleBetweenVector(Vector(0.0, 1.0, 0.0)) * ((direction_.x() < 0.0) ? 1.0 : -1.0), 0.0, 0.0, 1.0);
        
        // Definir o material activo que vai ser usado para o pacman - Amarelo baco
        GLfloat diffuseMaterialYellow[] = {1.0, 1.0, 0.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialYellow);
        GLfloat ambientMaterialYellow[] = {1.0, 1.0, 0.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialYellow);
        
        if (is_facing_up_ == true) {
            glRotatef(90.0, 0.0, 1.0, 0.0);
        } else {
            glRotatef(11.25, 1.0, 0.0, 0.0);
        }
        
        float munchingAngleInRadians = munching_angle_ * (M_PI / 180.0);
        
        // Quando o pacman fica parado fica com a boca mais aberta quando esta na vista topdown
        float angleVariation = (is_facing_up_ == true) ? cos(munchingAngleInRadians) : sin(munchingAngleInRadians);
        
        glPushMatrix();
        glRotatef(22.5 + angleVariation * 22.5, 1.0, 0.0, 0.0);
        Utilities::CreateSphere(PACMAN_RADIUS, PACMAN_RADIUS, PACMAN_RADIUS, ANGLE_I_STEP, ANGLE_J_STEP, 0, 90);
        if (is_facing_up_ == false) {
            glPushMatrix();
            
            // Definir o material activo que vai ser usado para a boca do pacman - Cor de laranja baco
            GLfloat diffuseMaterialOrange[] = {1.0, 0.5, 0.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialOrange);
            GLfloat ambientMaterialOrange[] = {1.0, 0.5, 0.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialOrange);
            
            Utilities::CreateDisk(PACMAN_RADIUS, PACMAN_RADIUS, ANGLE_I_STEP, 180);
            glPopMatrix();
        }
        glPopMatrix();
        
        // Definir o material activo que vai ser usado para o pacman - Amarelo baco
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialYellow);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialYellow);
        
        glPushMatrix();
        glRotatef(-22.5 - angleVariation * 22.5, 1.0, 0.0, 0.0);
        Utilities::CreateSphere(PACMAN_RADIUS, PACMAN_RADIUS, PACMAN_RADIUS, ANGLE_I_STEP, ANGLE_J_STEP, -90, 0);
        glRotatef((is_facing_up_ == true) ? 0.0 : 90.0, 1.0, 0.0, 0.0);
        
        // Os olhos do pacman nao sao desenhados se ele esta na vista topdown
        if (is_facing_up_ == false) {
            // Os olhos do pacman acompanham sempre a metade superior da sua boca
            glPushMatrix();
            glRotatef(-22.5, 1.0, 0.0, 0.0);
            eyes_->Display();
            eyebrows_->Display();
            glPopMatrix();
        }
        
        glPopMatrix();
        
        glPopMatrix();
    }
    
    if (particle_manager_ != 0) {
        particle_manager_->Display();
    }
}

void Pacman::Update(float deltaSeconds)
{    
    if (is_dead_ == false) { 
        if (is_munching_ == true || (is_munching_ == false && munching_angle_ != 0.0)) {
            munching_angle_ += MUNCHING_SPEED * 360.0 * deltaSeconds;
        }
        
        if (munching_angle_ > 360.0) {
            munching_angle_ = 0.0;
        }
        
        if (Utilities::IsIntersection(position_)) {
            // Caso em que o pacman esta numa interseccao
            distance_traveled_ = 0.0;
            Vector wantedPosition = position_ + wanted_direction_;
            int index = (int)Utilities::RoundNumber(wantedPosition.x()) + (int)Utilities::RoundNumber(wantedPosition.y()) * (int)maze_->width();
            char mazeBlock = maze_->data()[index];
            
            if (mazeBlock == PELLET || mazeBlock == SUPER_PELLET || mazeBlock == EMPTY_SPACE || mazeBlock == TELEPORT) {
                // Caso em que o pacman esta numa interseccao e a direccao que ele pretende seguir nao tem paredes
                if (mazeBlock == SUPER_PELLET) {
                    // Assusta os fantasmas se comer uma bola grande
                    this->ScareGhosts();
                }
                if (mazeBlock == PELLET) {
                    // Ganha pontos se comer uma bola pequena
                    score_ += PELLET_POINTS;
                    maze_->set_number_of_pellets(maze_->number_of_pellets() - 1);
                }
                Vector positionDelta = wanted_direction_ * speed_ * deltaSeconds;
                distance_traveled_ += positionDelta.Length();
                position_ += positionDelta;
                direction_ = wanted_direction_;
                if(mazeBlock != TELEPORT)
                    maze_->data()[index] = EMPTY_SPACE;
            } else {
                // Caso em que o pacman esta numa interseccao e a direccao que ele prefere fa-lo ir contra uma parede
                // Neste caso, testa-se entao a direccao que ele tinha anteriormente
                Vector wantedPosition = position_ + direction_;
                int index = (int)Utilities::RoundNumber(wantedPosition.x()) + (int)Utilities::RoundNumber(wantedPosition.y()) * (int)maze_->width();
                char mazeBlock = maze_->data()[index];
                
                if (mazeBlock == PELLET || mazeBlock == SUPER_PELLET || mazeBlock == EMPTY_SPACE || mazeBlock == TELEPORT) {
                    // Caso em que a direccao que o pacman levava nao o faz ir contra uma parede
                    if (mazeBlock == SUPER_PELLET) {
                        this->ScareGhosts();
                    }
                    if (mazeBlock == PELLET) {
                        score_ += PELLET_POINTS;
                        maze_->set_number_of_pellets(maze_->number_of_pellets() - 1);
                    }
                    Vector positionDelta = direction_ * speed_ * deltaSeconds;
                    distance_traveled_ += positionDelta.Length();
                    position_ += positionDelta;
                    if(mazeBlock != TELEPORT)
                        maze_->data()[index] = EMPTY_SPACE;
                } else {
                    // Caso em que o faz ir contra uma parede
                    is_munching_ = false;
                }
            }
        } else {
            // Caso em que o pacman nao esta numa interseccao
            Vector positionDelta = direction_ * speed_ * deltaSeconds;
            distance_traveled_ += positionDelta.Length();
            if (distance_traveled_ > 1.0) {
                // Caso em que o pacman nao estava numa interseccao mas entretanto vai passar por uma
                float remainingTravelingDistance = distance_traveled_ - 1.0;
                distance_traveled_ = 0.0;
                position_ = Utilities::NearestIntersection(position_);
                Vector wantedPosition = position_ + wanted_direction_;
                int index = (int)Utilities::RoundNumber(wantedPosition.x()) + (int)Utilities::RoundNumber(wantedPosition.y()) * (int)maze_->width();
                char mazeBlock = maze_->data()[index];
                if (mazeBlock == PELLET || mazeBlock == SUPER_PELLET || mazeBlock == EMPTY_SPACE || mazeBlock == TELEPORT) {
                    // Caso em que a posicao para onde o pacman quer ir nao esta ocupada por paredes
                    if (mazeBlock == SUPER_PELLET) {
                        this->ScareGhosts();
                    }
                    if (mazeBlock == PELLET) {
                        score_ += PELLET_POINTS;
                        maze_->set_number_of_pellets(maze_->number_of_pellets() - 1);
                    }
                    is_munching_ = true;
                    distance_traveled_ += remainingTravelingDistance;
                    position_ += wanted_direction_ * remainingTravelingDistance;
                    direction_ = wanted_direction_;
                    if(mazeBlock != TELEPORT)
                        maze_->data()[index] = EMPTY_SPACE;
                } else {
                    // Caso em que a posicao para onde o pacman quer ir esta ocupada por paredes portanto experimenta com a direccao que tinha antes
                    Vector wantedPosition = position_ + direction_;
                    int index = (int)Utilities::RoundNumber(wantedPosition.x()) + (int)Utilities::RoundNumber(wantedPosition.y()) * (int)maze_->width();
                    char mazeBlock = maze_->data()[index];
                    if (mazeBlock == PELLET || mazeBlock == SUPER_PELLET || mazeBlock == EMPTY_SPACE || mazeBlock == TELEPORT) {
                        // Caso em que a direccao que o pacman tinha nao bate numa parede
                        if (mazeBlock == SUPER_PELLET) {
                            this->ScareGhosts();
                        }
                        if (mazeBlock == PELLET) {
                            score_ += PELLET_POINTS;
                            maze_->set_number_of_pellets(maze_->number_of_pellets() - 1);
                        }
                        is_munching_ = true;
                        distance_traveled_ += remainingTravelingDistance;
                        position_ += direction_ * remainingTravelingDistance;
                        if(mazeBlock != TELEPORT)
                            maze_->data()[index] = EMPTY_SPACE;
                    } else {
                        // Caso em que bate
                        is_munching_ = false;
                    }
                }
            } else {
                // Caso em que o pacman nao estava numa interseccao e nao vai passar por uma entretanto
                position_ += positionDelta;
            }
        }
        
        // Testa se o pacman chegou as zonas de teleporte
        int index = (int)position_.x() + (int) position_.y() * (int)maze_->width();
        char mazeBlock = maze_->data()[index];
        
        if (position_.x() == (maze_->width() - 1.0) && mazeBlock == TELEPORT) {
            position_ = Vector(0.0, position_.y(), position_.z());
        } else if (position_.x() == 0.0 && mazeBlock ==  TELEPORT) {
            position_ = Vector(maze_->width() - 1.0, position_.y(), position_.z());
        }
    }
    
    if (particle_manager_->is_active() == true) {
        particle_manager_->Update(deltaSeconds);
    } else if (particle_manager_->active_particles() == 0) {
        is_dead_ = false;
        position_ = initial_position_;
        speed_ = 0.0;
        is_munching_ = false;
        munching_angle_ = 0.0;
        particle_manager_->set_active_particles(MAXIMUM_PARTICLES);
        direction_ = Vector(0.0, 1.0, 0.0);
        wanted_direction_ = Vector(0.0, 1.0, 0.0);
    }
}

void Pacman::RegisterGhost(Ghost *ghost)
{
    Pacman::ghosts.push_back(ghost);
}

void Pacman::ScareGhosts(void) const
{
	for (unsigned int i = 0; i < ghosts.size(); ++i) {
        // Coloca os fantasmas assustados com tempo de GHOSTS_SCARED_TIME segundos
        Pacman::ghosts[i]->set_is_scared(true, GHOSTS_SCARED_TIME);
    }
}

void Pacman::MoveLeft(float speed)
{
    wanted_direction_ = Vector(-1.0, 0.0, 0.0);
	speed_ = speed;
}

void Pacman::MoveRight(float speed)
{
    wanted_direction_ = Vector(1.0, 0.0, 0.0);
	speed_ = speed;
}

void Pacman::MoveDown(float speed)
{
    wanted_direction_ = Vector(0.0, 1.0, 0.0);
	speed_ = speed;
}

void Pacman::MoveUp(float speed)
{
    wanted_direction_ = Vector(0.0, -1.0, 0.0);
	speed_ = speed;
}
