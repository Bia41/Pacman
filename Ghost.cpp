#include "Ghost.hpp"

static const float DEAD_TIME = 5.0;

void Ghost::CreateGhost(float radius, float height, int numberOfLegs) const
{
    for (int i = 0; i <= 360; i += ANGLE_I_STEP) {
        // Parte em que cria a cabeca do fantasma
        glBegin(GL_TRIANGLE_STRIP);
        float u = (float)i * (M_PI / 180.0);
        float w = (float)(i + ANGLE_I_STEP) * (M_PI / 180.0);
        for (int j = 90; j >= 0; j -= ANGLE_J_STEP) {
            float v = (float)j * (M_PI / 180.0);
            glNormal3f(cos(v) * cos(u),
                       cos(v) * sin(u),
                       sin(v));
            glVertex3f(radius * cos(v) * cos(u),
                       radius * cos(v) * sin(u),
                       radius * sin(v));
            
            glNormal3f(cos(v) * cos(w),
                       cos(v) * sin(w),
                       sin(v));
            glVertex3f(radius * cos(v) * cos(w),
                       radius * cos(v) * sin(w),
                       radius * sin(v));
        }
        
        // Parte em que cria as patinhas do fantasma
        glNormal3f(cos(u), sin(u), 0.0);
        glVertex3f(radius * cos(u),
                   radius * sin(u),
                   -height + cos(numberOfLegs * u) * 0.15);
        
        glNormal3f(cos(w), sin(w), 0.0);
        glVertex3f(radius * cos(w),
                   radius * sin(w),
                   -height + cos(numberOfLegs  * w) * 0.15);
        glEnd();
    }
}

Ghost::Ghost(Vector position, Maze *maze, Pacman *pacman, bool isFacingUp, Vector normalColor)
{
    position_ = position;
    initial_position_ = position;
    distance_traveled_ = 0.0;
    direction_ = Vector(1.0, 0.0, 0.0);
    stalking_direction_ = Vector(0.0, 1.0, 0.0);
    
    is_facing_up_ = isFacingUp;
    maze_ = maze;
    pacman_ = pacman;
    normal_color_ = normalColor;
    is_scared_ = false;
    is_dead_ = false;
    
    scared_time_ = 0.0;
    dead_time_ = 0.0;
    
    speed_ = PACMAN_SPEED;
    eyes_ = new Eyes();
    
    display_list_ = glGenLists(1);
    glNewList(display_list_, GL_COMPILE);
    glPushMatrix();
    
    // Cria um fantasma com GHOST_RADIUS de raio na cabeca e corpo e com 6 patinhas
    this->CreateGhost(GHOST_RADIUS, GHOST_RADIUS, 6);
    
    glPopMatrix();
    glEndList();
}

Ghost::~Ghost(void)
{
    delete eyes_;
    glDeleteLists(display_list_, 1);
}

void Ghost::Die(void)
{
    is_dead_ = true;
    dead_time_ = DEAD_TIME;
    position_ = initial_position_;
    speed_ = PACMAN_SPEED;
    
}

bool Ghost::CheckPacmanCollision(void)
{
    return ((pacman_->position()).Distance(position_) <= (PACMAN_RADIUS + GHOST_RADIUS));
}

Vector Ghost::GetStalkingDirection(std::vector<Vector> possiblePositions)
{
	Vector closestPosition = possiblePositions[0];
	float minDistance = pacman_->position().Distance(possiblePositions[0]);
	for(unsigned int i = 1; i < possiblePositions.size(); i++){
		float distance = pacman_->position().Distance(possiblePositions[i]);
		if(distance < minDistance){
			minDistance = distance;
			closestPosition = possiblePositions[i];
		}
	}
	return closestPosition - position_;
}

Vector Ghost::GetRunningDirection(std::vector<Vector> possiblePositions)
{
	Vector furthestPosition = possiblePositions[0];
	float maxDistance = pacman_->position().Distance(possiblePositions[0]);
	for(unsigned int i = 1; i < possiblePositions.size(); i++){
		float distance = pacman_->position().Distance(possiblePositions[i]);
		if(distance > maxDistance){
			maxDistance = distance;
			furthestPosition = possiblePositions[i];
		}
	}
	return furthestPosition - position_;
}

Vector Ghost::CheckPosition(float angle) {
	Vector possibleDirection = Utilities::NearestIntersection(direction_.Rotated(angle));
	Vector possiblePosition = position_ + possibleDirection;
	int index = (int)Utilities::RoundNumber(possiblePosition.x()) + (int)Utilities::RoundNumber(possiblePosition.y()) * (int)maze_->width();
	char mazeBlock = maze_->data()[index];
    
	if (mazeBlock == PELLET || mazeBlock == SUPER_PELLET || mazeBlock == EMPTY_SPACE || mazeBlock == TELEPORT || mazeBlock == DOOR) {
		return possiblePosition;
	}
	else return Vector(-1.0, -1.0, -1.0);
}

std::vector<Vector> Ghost::GetPossiblePositions(void){
	std::vector<Vector> possiblePositions;
	
	Vector pos = CheckPosition(90.0);
	if(pos != Vector(-1.0, -1.0, -1.0)){
		possiblePositions.push_back(pos);
	}
	pos = CheckPosition(270.0);
	if(pos != Vector(-1.0, -1.0, -1.0)){
		possiblePositions.push_back(pos);
	}
	if (possiblePositions.size() > 0) {
		pos = CheckPosition(0.0);
		if(pos != Vector(-1.0, -1.0, -1.0)){
			possiblePositions.push_back(pos);
		}
		pos = CheckPosition(180.0);
		if(pos != Vector(-1.0, -1.0, -1.0)){
			possiblePositions.push_back(pos);
		}
	} else if (!(position_.x() >= (maze_->width() - 1.0) || position_.x() <= 0.0) 
               && CheckPosition(0.0) == Vector (-1.0, -1.0, -1.0)){
        possiblePositions.push_back(CheckPosition(180.0));
    }
    return possiblePositions;	
} 


void Ghost::Display(void) const
{
    if (is_dead_ == false) {
        glPushMatrix();
        
        glTranslatef(position_.x(), position_.y(), position_.z());
        
        if (is_facing_up_ == false) {
            // Calcula angulo entre direccao do fantasma e o eixo dos yy, para poder roda-lo sobre o eixo dos zz e ficar a olhar para o pacman
            glRotatef(stalking_direction_.AngleBetweenVector(Vector(0.0, -1.0, 0.0)) * ((stalking_direction_.x() < 0.0) ? -1.0 : 1.0), 0.0, 0.0, 1.0);
        } else {
            // No caso de estar na perspectiva topdown apenas vira o fantasma para olhar para a frente
            glRotatef(-90.0, 1.0, 0.0, 0.0);
        }
        
        glPushMatrix();
        glRotatef(90.0, 1.0, 0.0, 0.0);
        eyes_->Display();
        glPopMatrix();
        
        glRotatef(180.0, 1.0, 0.0, 0.0);
        if (is_scared_ == true) {
            // Definir o material activo que vai ser usado para o fantasma quando esta assustado - Branco baco
            GLfloat diffuseMaterialWhite[] = {1.0, 1.0, 1.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialWhite);
            GLfloat ambientMaterialWhite[] = {1.0, 1.0, 1.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialWhite);
        } else {
            // Definir o material activo que vai ser usado para o fantasma quando nao esta assustado - Cor do fantasma
            GLfloat diffuseMaterialColored[] = {normal_color_.x(), normal_color_.y(), normal_color_.z()};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialColored);
            GLfloat ambientMaterialColored[] = {normal_color_.x(), normal_color_.y(), normal_color_.z()};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialColored);
        }
        glCallList(display_list_);
        
        glPopMatrix();
    }
}

void Ghost::Update(float deltaSeconds)
{
    if (is_scared_ == true) {
        // Se o fantasma esta assustado vai-lhe subtraindo os intervalos de tempo
        scared_time_ -= deltaSeconds;
        
        if (scared_time_ <= 0.0) {
            // Quando o tempo acaba o fantasma deixa de estar assustado
            scared_time_ = 0.0;
            is_scared_ = false;
            speed_ = PACMAN_SPEED;
        }
    }
    
    if (is_dead_ == true) {
        dead_time_ -= deltaSeconds;
        
        if (dead_time_ <= 0.0) {
            dead_time_ = 0.0;
            is_dead_ = false;
            is_scared_ = false;
            scared_time_ = 0.0;
            speed_ = PACMAN_SPEED;
        }
    }
    
    // A direccao para onde o fantasma olha e sempre a apontar para o pacman
    stalking_direction_ = position_ - pacman_->position();
    
    if (is_dead_ == false) {
        if (Utilities::IsIntersection(position_)) {
            distance_traveled_ = 0.0;
            std::vector<Vector> possiblePositions = GetPossiblePositions();		
            
            if (possiblePositions.size() > 0) {
                if ((rand() % 100 + 1) > 25) {
                    if(is_scared_)
                        direction_ = GetRunningDirection(possiblePositions);
                    else {
                        direction_ = GetStalkingDirection(possiblePositions);
                    }
                } else {
                    direction_ = Utilities::NearestIntersection(direction_.Rotated(180.0));
                }
            }
            
            position_ += direction_ * speed_ * deltaSeconds;
        } else {
            Vector positionDelta = direction_ * speed_ * deltaSeconds;
            distance_traveled_ += positionDelta.Length();
            
            if (distance_traveled_ > 1.0) {
                float remainingTravelingDistance = distance_traveled_ - 1.0;
                distance_traveled_ = 0.0;
                position_ = Utilities::NearestIntersection(position_);
                std::vector<Vector> possiblePositions = GetPossiblePositions();	
                
                if (possiblePositions.size() > 0) {
                    if ((rand() % 100 + 1) > 25) {
                        if(is_scared_){
                            direction_ = GetRunningDirection(possiblePositions);
                            
                        }else {
                            
                            direction_ = GetStalkingDirection(possiblePositions);
                        }
                    } else {
                        direction_ = Utilities::NearestIntersection(direction_.Rotated(180.0));
                    }
                }
                distance_traveled_ += remainingTravelingDistance;
                position_ += direction_ * remainingTravelingDistance;
            } else {
                // Caso em que o fantasma nao estava numa interseccao e nao vai passar por uma entretanto
                position_ += positionDelta;
            }
        }
        
        // Testa se o fantasma chegou as zonas de teleporte
        int index = (int)position_.x() + (int) position_.y() * (int)maze_->width();
        char mazeBlock = maze_->data()[index];
        
        if (position_.x() >= (maze_->width() - 1.0) && mazeBlock == TELEPORT) {
            position_ = Vector(0.0, position_.y(), position_.z());
        } else if (position_.x() <= 0.0 && mazeBlock ==  TELEPORT) {
            position_ = Vector(maze_->width() - 1.0, position_.y(), position_.z());
        }
        
        if (CheckPacmanCollision()) {
            if (is_scared_ == true) {
                Die();
            } else {
                if (pacman_->is_dead() == false) {
                    pacman_->Die();
                }
            }
        }
    }
}
