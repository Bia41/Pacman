#include "Maze.hpp"

// Funcao para desenhar um canto do labirinto
// Recebe um angulo para poder variar o canto que se desenha so com um argumento
void Maze::DisplayCorner(float angle, int i, int j) const
{
    glPushMatrix();
    
    glTranslatef((float)j, (float)i, 0.0);
    
    glRotatef(-angle, 0.0, 0.0, 1.0);
    glTranslatef(0.5, 0.5, 0.0);
    
    Utilities::CreateTorus(0.25, 0.25, 0.25, 0.5, ANGLE_I_STEP, ANGLE_J_STEP, 180, 270);
    
    glPopMatrix();
}

// Construtor do labirinto - recebe o nome do ficheiro com a informacao para o criar
Maze::Maze(const std::string fileName)
{
    number_of_pellets_ = 0;
    
    // Carrega a textura do chao no OpenGL e guarda o handle para ela
    floor_texture_name_ = TextureManager::GetTextureManager()->GetTextureName(std::string("Resources/Floor.tga"));

    // Abrir o ficheiro para leitura
    std::ifstream file(fileName.c_str(), std::ios_base::in);
    
    // Ler a largura do labirinto
    file >> width_;
    
    // Ignorar o "new line"
    file.ignore(1);
    
    // Ler a altura do labirinto
    file >> height_;
    
    // Ignorar o "new line"
    file.ignore(1);
    
    // Criar um array para guardar a informacao do labirinto como caracteres
    data_ = new char[width_ * height_];
    
    // Ler a informacao do ficheiro para o array
    for (int i = 0; i < height_; ++i) {
        file.read(&(data_[width_ * i]), width_);
        
        // Ignorar o "new line" de cada linha do labirinto
        file.ignore(1);
    }
    
    file.close();
    
    // Pedir ao OpenGL para gerar um identificador para uma display list e cria-la
    maze_display_list_ = glGenLists(1);
    glNewList(maze_display_list_, GL_COMPILE);
    
    // Todos os comandos seguintes serao compilados na display list para melhorar a performance
    
    // Definir o material activo que vai ser usado para os cantos e paredes - Azul escuro baco
    GLfloat diffuseMaterialBlue[] = {0.0, 0.0, 0.5};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialBlue);
    GLfloat ambientMaterialBlue[] = {0.0, 0.0, 0.5};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialBlue);
    
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            switch (data_[j + i * width_]) {
                case LEFT_TOP_CORNER:
                    this->DisplayCorner(0.0, i, j); 
                    break;
                case LEFT_BOTTOM_CORNER:
                    this->DisplayCorner(90.0, i, j);
                    break;
                case RIGHT_BOTTOM_CORNER:
                    this->DisplayCorner(180.0, i, j);
                    break;
                case RIGHT_TOP_CORNER:
                    this->DisplayCorner(270.0, i, j);
                    break;
                case VERTICAL_WALL:
                    glPushMatrix();
                    glTranslatef((float)j, (float)i, 0.0);
                    // As paredes sao cilindros sem os topos
                    Utilities::CreateCylinder(0.25, 0.25, ANGLE_I_STEP);
                    glPopMatrix();
                    break;
                case HORIZONTAL_WALL:
                    glPushMatrix();
                    glTranslatef((float)j, (float)i, 0.0);
                    glRotatef(90.0, 0.0, 0.0, 1.0);
                    Utilities::CreateCylinder(0.25, 0.25, ANGLE_I_STEP);
                    glPopMatrix();
                    break;
                case PELLET:
                    ++number_of_pellets_;
                    break;
                default:
                    break;
            }
        }
    }
    
    // Definir o material activo que vai ser usado para as portas - Cor de rosa baco
    GLfloat diffuseMaterialPink[] = {1.0, 0.75, 0.75};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialPink);
    GLfloat ambientMaterialPink[] = {1.0, 0.75, 0.75};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialPink);
    
    // Cria as portas a parte para nao ter que fazer mudanca de material
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            switch (data_[j + i * width_]) {
                case DOOR:
                    // As portas sao as paredes do centro por onde passam os fantasmas
                    glPushMatrix();
                    glTranslatef((float)j, (float)i, 0.0);
                    glRotatef(90.0, 0.0, 0.0, 1.0);
                    Utilities::CreateCylinder(0.25, 0.25, ANGLE_I_STEP);
                    glPopMatrix();
                    break;
                default:
                    break;
            }
        }
    }
    
    glPushMatrix();
    
    // Transforma o chao do labirinto para bater certo com o labirinto
    glTranslatef(-0.5, -0.5, 1.0);
    
    // Definir o material activo que vai ser usado para o chao - Cinzento baco
    GLfloat diffuseMaterialGrey[] = {0.75, 0.75, 0.75};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialGrey);
    GLfloat ambientMaterialGrey[] = {0.75, 0.75, 0.75};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialGrey);
    
    // Activa o estado para desenhar texturas nos vertices
    glEnable(GL_TEXTURE_2D);

    // Torna a textura do chao activa
    glBindTexture(GL_TEXTURE_2D, floor_texture_name_);
    
    // O chao e um quadrado do tamanho do labirinto
    Utilities::CreateSquare(width_, height_, 360, 360, true, 4.0);
    
    glPopMatrix();
    
    // Desactiva o estado de desenhar texturas depois de desenhar o chao
    glDisable(GL_TEXTURE_2D);

    // Acaba a compilacao da display list do labirinto
    glEndList();
    
    // Cria a display list dos pellets
    pellet_display_list_ = glGenLists(1);
    glNewList(pellet_display_list_, GL_COMPILE);
    
    // Os pellets sao esferas com 0.25 de raio
    Utilities::CreateSphere(0.25, 0.25, 0.25, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glEndList();
    
    // Cria a display list dos super pellets
    super_pellet_display_list_ = glGenLists(1);
    glNewList(super_pellet_display_list_, GL_COMPILE);

    // Os super pellets sao esferas com 0.5 de raio
    Utilities::CreateSphere(0.5, 0.5, 0.5, ANGLE_I_STEP, ANGLE_J_STEP, -90, 90);
    glEndList();
}

Maze::~Maze(void)
{
    delete[] data_;
    glDeleteLists(super_pellet_display_list_, 1);
    glDeleteLists(pellet_display_list_, 1);
    glDeleteLists(maze_display_list_, 1);
}

void Maze::Display(void) const
{
    glPushMatrix();
    
    // Define o nivel de brilho das esferas pois elas teem um aspecto metalico
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 48);

    // Define os componentes ambiente, difuso e especular dos materiais das esferas que representam a quantidade dos componentes da luz que sao reflectidos
    // Os pellets usam um material parecido com prata
    GLfloat specularMaterialSilver[] = {1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterialSilver);
    GLfloat diffuseMaterialSilver[] = {0.5, 0.5, 0.5};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialSilver);
    GLfloat ambientMaterialSilver[] = {0.25, 0.25, 0.25};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialSilver);
    
    // Os pellets e super pellets sao desenhados a parte do labirinto pois e provavel que sejam alterados
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            switch (data_[j + i * width_]) {
                case PELLET:
                    glPushMatrix();
                    glTranslatef((float)j, (float)i, 0.0);
                    glCallList(pellet_display_list_);
                    glPopMatrix();
                    break;
                default:
                    break;
            }
        }
    }
    
    // Os super pellets usam um material parecido com ouro
    GLfloat specularMaterialGold[] = {1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterialGold);
    GLfloat diffuseMaterialGold[] = {0.5, 0.5, 0.25};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialGold);
    GLfloat ambientMaterialGold[] = {0.25, 0.25, 0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialGold);
    
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            switch (data_[j + i * width_]) {
                case SUPER_PELLET:
                    glPushMatrix();
                    glTranslatef((float)j, (float)i, 0.0);
                    glCallList(super_pellet_display_list_);
                    glPopMatrix();
                    break; 
                default:
                    break;
            }
        }
    }
    
    // Colocamos o componente especular dos materiais a nao reflectir nada pois o resto dos objectos sao bacos - apenas as esferas teem brilho
    // Se for preciso colocar outros objectos com brilho, esta chamada a funcao deve sair daqui e passa-se a incluir a parte especular dos materiais em todos os objectos
    GLfloat specularMaterialNone[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterialNone);

    glPopMatrix();
    
    // Chama a display list do labirinto sempre que quer desenhar as partes fixas - paredes, portas e cantos
    glCallList(maze_display_list_);
}
