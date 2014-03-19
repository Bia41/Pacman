#include "OpenGL.hpp"

#include <cstdlib>

#include "Maze.hpp"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "TextureManager.hpp"

// Codigo ASCII da tecla Escape
static const unsigned char ESCAPE_KEY = 27;

static const float MILLISECONDS_PER_SECOND = 1000.0;

// Numero de milisegundos de intervalo pretendido entre frames
static const int DESIRED_FRAMES_PER_SECOND_INTERVAL = 5;

// Tempo em segundos de um ciclo de update
static const float UPDATE_TIME_STEP = 0.04;

// Cadeias de caracteres para escrever pontuacao e numero de vidas
static const char scoreString[] = "Pontuacao: ";
static const char livesString[] = "Vidas: ";
static const char gameOverString[] = "GAME OVER!";
static const char gameWinString[] = "YOU WIN!";


// Array onde se converte a pontuacao antes de a desenhar
static char scoreArray[10];

static bool isMatrixMode = false;
static bool isGameOver = false;
static bool isGameWin = false;


static Maze *maze = 0;
static Pacman *pacman = 0;
static Ghost *ghosts[3];

// Flag que define se a camara ve o labirinto de cima ou se esta a seguir o pacman
static bool isTopdownCamera = false;

// Flag que define se a luz e direccional ou se e uma spotlight na posicao do pacman e que aponta para onde ele olha
static bool isDirectionalLight = true;

static int windowWidth = 400;
static int windowHeight = 400;

// Serve para guardar o tempo que passa entre invocacoes do update (atraves da funcao Timer())
static int elapsedTime = 0;

// Funcao que e registada no glutDisplayFunc() e e chamada no ciclo sempre que a aplicacao quer desenhar no ecra
void Display(void)
{
    // Relacao de aspecto
    double aspectRatio = (double)windowWidth / (double)windowHeight;
    
    // Valores utilizados para centrar a camara no labirinto
    double halfMazeWidth = maze->width() / 2.0;
    double halfMazeHeight = maze->height() / 2.0;
    
    // Define que as proximas alteracoes na matriz serao na de projeccao
    glMatrixMode(GL_PROJECTION);
    
    // Carrega a matriz identidade na matriz de projeccao
    glLoadIdentity();
    
    if(isTopdownCamera == true) {
        // Define uma projeccao ortogonal com um volume visivel entre - e + metade do tamanho do labirinto no eixo dos xx e dos
        // yy e de -2.0 a 2.0 no eixo dos zz tendo em conta a relacao de aspecto
        if (aspectRatio >= 1.0) {
            glOrtho(-halfMazeWidth * aspectRatio, halfMazeWidth * aspectRatio, -halfMazeHeight, halfMazeHeight, -2.0, 2.0);
        } else {
            glOrtho(-halfMazeWidth, halfMazeWidth, -halfMazeHeight / aspectRatio, halfMazeHeight / aspectRatio, -2.0, 2.0);
        }
        
        // Definimos o modelo de luz para nao ter um ponto de vista local (modelo por defeito)
        // Isto deve-se ao ponto de vista estar a uma distancia infinita pois projeccao ortogonal implica raios paralelos a partir do ponto de vista
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    } else {
        // Se a camara e a que segue o pacman entao definimos uma projeccao perspectiva
        gluPerspective(45.0, aspectRatio, 0.1, 75.0);
        
        // Definimos o modelo de luz para ter um ponto de vista local, de forma a calcular bem as reflexoes derivadas da componente especular das luzes e dos materiais
        // Caso contrario, usa como direccao do ponto de vista o vector (0, 0, 1)
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	}
    
    // Define a cor com que se limpa o ecra antes de desenhar
    glClearColor(0.75, 0.75, 0.75, 1.0); // Cinzento
    
    // Limpar os buffers onde se desenhou e o do teste das profundidades
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Mudar o stack de matrizes para a modelview
    glMatrixMode(GL_MODELVIEW);
    
    // Carregar a matriz identidade em modelview
    glLoadIdentity();
    
    if (isTopdownCamera == false) {
        // Move a camara para estar sempre centrada no pacman e ligeiramente acima e a frente deste
        Vector cameraPosition = pacman->position() + Vector(0.0, 15.0, -15.0);
        gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(), pacman->position().x(), pacman->position().y(), pacman->position().z(), 0.0, -1.0, -1.0);
    } else {
        // Movimenta a camara para ficar centrada no labirinto e roda-a 180 graus sobre o eixo dos xx para o eixo dos yy ficar virado para baixo
        glRotatef(180.0, 1.0, 0.0, 0.0);
        glTranslatef(-halfMazeWidth + 0.5, -halfMazeHeight + 0.5, 0.0);
    }
    
    if (isDirectionalLight == true) {
    	// No caso em que a luz e direccional definimos a direccao da luz 0
    	// Como o ultimo parametro e 0.0, a luz e direccional e os tres primeiros parametros definem a sua direccao
    	// Ter em conta que o valor por defeito e (0.0, 0.0, 1.0, 0.0) e que isso define uma luz paralela e em direccao ao eixo -zz
        GLfloat lightDirection[] = {0.0, 0.0, -1.0, 0.0};
        glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
        
		// Definimos o angulo de iluminacao da luz 0 para ser igual ao angulo por defeito e iluminar a toda a volta
		// Esta chamada e apenas necessaria nos casos em que ja foi alterado esse angulo, ou seja, quando alteramos o tipo de iluminacao de em foco para direccional
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
	} else {
        // No caso em que a luz esta na cabeca do pacman
		// Definimos o angulo de iluminacao da luz 0 para fazer um foco
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
        
        // Define a constante de atenuacao da luz 0
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0);
		
		// Definimos a posicao da luz 0 para ser a do pacman - o parametro 1.0 torna a luz posicional
		GLfloat lightPosition[] = {pacman->position().x(), pacman->position().y(), pacman->position().z(), 1.0};
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        
		// Definimos a direccao da luz 0 para ser igual a do pacman
		GLfloat lightDirection[] = {pacman->direction().x(), pacman->direction().y(), pacman->direction().z()};
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
    }
    
    // Desenha o labirinto, o pacman e os fantasmas
    maze->Display();
    pacman->Display();
    
    for (int i = 0; i < 3; ++i) {
        ghosts[i]->Display();
    }
    
    // Desactiva as luzes para escrever a GUI
    glDisable(GL_LIGHTING);
    
    // Desactiva o teste de profundidade para desenhar a GUI
    glDisable(GL_DEPTH_TEST);
    
    // Carrega a matriz identidade na matriz de transformacao de projeccao
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
    // Define o volume de visualizacao para ser do tamanho da janela
	glOrtho(0.0, (GLfloat)windowWidth, 0.0, (GLfloat)windowHeight, -11.0, 11.0);
	
    // Carrega a matriz identidade na matriz modelview
    glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
    
    // Define a cor para escrever o texto no ecra
	glColor3f(1.0, 1.0, 1.0); // Vermelho
    
    
    if ((isGameOver == false) && (isGameWin == false)) {
        
        // Define a posicao do cursor onde se vai comecar a escrever para ser no topo do ecra
        glRasterPos2i(10, windowHeight - 15);
        
        // Desenha a string da pontuacao
        for (int i = 0; scoreString[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, scoreString[i]);
        }
        
        // Converte a pontuacao de inteiro para string
        sprintf(scoreArray, " %d ", pacman->score());
        
        // Desenha a pontuacao no ecra
        for (int i = 0; scoreArray[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, scoreArray[i]);
        }
        
        // Define a posicao do cursor onde se vai escrever para ser no topo do ecra mas um bocado mais a direita
        glRasterPos2i(windowWidth - 85, windowHeight - 15);
        
        // Desenha a string das vidas no ecra
        for (int i = 0; livesString[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, livesString[i]);
        }
        
        // Activa de novo o teste de profundidade
        glEnable(GL_DEPTH_TEST);
        
        // Altera a cor para desenhar as vidas
        glColor3f(1.0, 1.0, 0.0); // Amarelo
        
        // Posiciona o desenho das vidas no canto superior direito
        glTranslatef(windowWidth - 10.0, windowHeight - 10.0, 0.0);
        
        // Desenha o numero de vidas no ecra    
        for (int i = 0; i < pacman->lives() ; ++i) {
            // Cada vida e um pacman de boca aberta
            glPushMatrix();
            glTranslatef(i * -15.0, 0.0, 0.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            
            glPushMatrix();
            glRotatef(45.0, 1.0, 0.0, 0.0);
            Utilities::CreateSphere(5.0, 5.0, 5.0, ANGLE_I_STEP, ANGLE_J_STEP, 0, 90);
            glPopMatrix();
            
            glRotatef(-45.0, 1.0, 0.0, 0.0);
            Utilities::CreateSphere(5.0, 5.0, 5.0, ANGLE_I_STEP, ANGLE_J_STEP, -90, 0);
            
            glPopMatrix();
        }
    }
    else if (isGameOver == true) {
        glDisable(GL_DEPTH_TEST);
        
        glRasterPos2i(windowWidth /2 -55, windowHeight / 2);
        for (int i = 0; gameOverString[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverString[i]);
        }
        glEnable(GL_DEPTH_TEST);
        
    } else if (isGameWin == true) {
        glDisable(GL_DEPTH_TEST);
        
        glRasterPos2i(windowWidth /2 -55, windowHeight / 2);
        for (int i = 0; gameWinString[i] != '\0'; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameWinString[i]);
        }
        glEnable(GL_DEPTH_TEST);
    }
    // Activa as luzes
    glEnable(GL_LIGHTING);
    
    // Trocar o buffer onde se estava a desenhar pelo que vai aparecer no ecra
    // Nao esquecer que a aplicacao utiliza dois buffers para desenhar - double buffered
    glutSwapBuffers();
}

// Funcao que e registada no glutReshapeFunc() e e chamada sempre que se altera o tamanho da janela ou quando esta e criada
void Reshape(int width, int height)
{
    // Define a porcao da janela que vai ser usada para desenhar
    // Neste caso usa-se a janela toda
    glViewport(0, 0, width, height);
    
    // Guarda os novos valores da altura e largura da janela
    windowWidth = width;
    windowHeight = height;
}

// Funcao que e chamada sempre que o temporizador da sinal e que serve para fazer update ao jogo
void Timer(int value)
{
    // Antes de tudo, regista esta funcao para ser chamada de novo daqui a DESIRED_FRAMES_PER_SECOND milisegundos (ou assim que possa)
    glutTimerFunc(DESIRED_FRAMES_PER_SECOND_INTERVAL, &Timer, value);
    
    // Milisegundos que passaram desde a ultima invocacao desta funcao - servem para ter a certeza que a aplicacao e independente da velocidade do processador
    int deltaMilliseconds = glutGet(GLUT_ELAPSED_TIME) - elapsedTime;
    elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    
    // Converter o valor para segundos
    float deltaSeconds = deltaMilliseconds / MILLISECONDS_PER_SECOND;
    
    if (isMatrixMode == true) {
        deltaSeconds *= 0.25;
    }
    
    if ((isGameOver == true) || (isGameWin == true)) {
        deltaSeconds = 0.0;
    }
    
    while (deltaSeconds > UPDATE_TIME_STEP) {
        // Fazer o update da movimentacao do pacman e dos fantasmas com pequenas variacoes de tempo para nao haver problemas com colisoes
        pacman->Update(UPDATE_TIME_STEP);
        for (int i = 0; i < 3; ++i) {
            ghosts[i]->Update(UPDATE_TIME_STEP);
        }
        deltaSeconds -= UPDATE_TIME_STEP;
    }
    
    // Faz o update mas agora com o tempo que sobrou do ciclo anterior
    pacman->Update(deltaSeconds);
    
    for (int i = 0; i < 3; ++i) {
        ghosts[i]->Update(deltaSeconds);
    }
    
    if(pacman->is_dead() == true) {
        isMatrixMode = true;
    } else {
        isMatrixMode =false;
    }
    
    if(pacman->lives() <= 0) {
        isGameOver =true;
    }
    
    if (maze->number_of_pellets() == 0) {
        isGameWin = true;
    }
    
    // Diz ao glut para chamar a funcao de Display() de novo
    glutPostRedisplay();
    
}

// Funcao que e registada no glutKeyboardFunc() e e chamada sempre que se toca numa tecla "normal"
void Keyboard(unsigned char key, int x, int y)
{
    // Se o utilizador pressiona Escape sai-se do programa assinalando ao SO que tudo correu bem
    if (key == ESCAPE_KEY) {
        delete pacman;
        delete maze;
        
        for (int i = 0; i < 3; ++i) {
            delete ghosts[i];
        }
        
        TextureManager::DeleteTextureManager();
        
        exit(EXIT_SUCCESS);
    }
    
    if (key == '5') {
        isMatrixMode = !isMatrixMode;
    }
    
    // A tecla '1' poe a camara a ver o jogo de cima com projeccao ortogonal
    if (key == '1') {
        isTopdownCamera = true;
    }
    
    // A tecla '2' faz a camara seguir o pacman e altera a projeccao para perspectiva
    if (key == '2') {
        isTopdownCamera = false;
    }
    
    // Avisar o pacman e os fantasmas que o tipo de camara mudou
    if (key == '1' || key == '2') {
        pacman->set_is_facing_up(isTopdownCamera);
        for (int i = 0; i < 3; ++i) {
            ghosts[i]->set_is_facing_up(isTopdownCamera);
        }
    }
    
	// A tecla '3' faz a luz ser direccional
	if (key == '3') {
		isDirectionalLight = true;
	}
    
	// A tecla '4' faz a luz ser em foco
	if (key == '4') {
		isDirectionalLight = false;
	}
}

// Funcao que e registada no glutSpecialFunc() e e chamada sempre que se toca numa tecla "especial" como as "setas" do teclado
void Special(int key, int x, int y)
{
    // As teclas cima, baixo, esquerda e direita movem o pacman nessa direccao
    switch (key) {
        case GLUT_KEY_UP:
            pacman->MoveUp(PACMAN_SPEED);
            break;
        case GLUT_KEY_DOWN:
            pacman->MoveDown(PACMAN_SPEED);
            break;
        case GLUT_KEY_LEFT:
            pacman->MoveLeft(PACMAN_SPEED);
            break;
        case GLUT_KEY_RIGHT:
            pacman->MoveRight(PACMAN_SPEED);
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    // Inicializa o glut
    glutInit(&argc, argv);
    
    // Diz ao glut quais vao ser as definicoes da janela
    // Neste caso queremos uma com 2 buffers para poder desenhar num e depois trocar pelo que vai para o ecra - GLUT_DOUBLE
    // O buffer onde se desenha vai ter o formato RGB, com um byte para cada componente de cor
    // Queremos tambem um buffer para fazer o teste de profundidade
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Define o tamanha da janela
    glutInitWindowSize(windowWidth, windowHeight);
    
    // Cria uma janela com o nome "Pacman" e as definicoes das funcoes anteriores
    glutCreateWindow("Pacman");
    
    // Regista as funcoes de desenho, redimensionamento da janela e de processamento do input
    glutDisplayFunc(&Display);
    glutReshapeFunc(&Reshape);
    glutKeyboardFunc(&Keyboard);
    glutSpecialFunc(&Special);
    
    // Cria um labirinto baseado no ficheiro dado
    maze = new Maze(std::string("Resources/Maze1.txt"));
    
    // Cria o pacman e coloca-o numa posicao livre do labirinto
    pacman = new Pacman(Vector(15.0, 18.0, 0.0), maze, isTopdownCamera);
    
    // Cria os fantasmas, cada um com uma cor diferente e coloca-os no meio do labirinto
    ghosts[0] = new Ghost(Vector(14.0, 15.0, 0.0), maze, pacman, isTopdownCamera, Vector(1.0, 0.5, 0.0));
    ghosts[1] = new Ghost(Vector(15.0, 15.0, 0.0), maze, pacman, isTopdownCamera, Vector(1.0, 0.75, 0.75));
    ghosts[2] = new Ghost(Vector(16.0, 15.0, 0.0), maze, pacman, isTopdownCamera, Vector(1.0, 0.0, 0.0));
    
    // A classe do pacman regista os fantasmas para lhes poder mandar mensagens (nomeadamente, assusta-los)
    for (int i = 0; i < 3; ++i) {
        Pacman::RegisterGhost(ghosts[i]);
    }
    
    // Activa o teste de profundidades do OpenGL dos pixeis que se vao desenhando para o desenho nao ficar com artefactos visiveis
    glEnable(GL_DEPTH_TEST);
    
    // Regista a funcao de update para ser chamada daqui a DESIRED_FRAMES_PER_SECOND milisegundos e dar inicio aos updates
    glutTimerFunc(DESIRED_FRAMES_PER_SECOND_INTERVAL, &Timer, 0);
    
    // Define o modelo de luz ambiente para nao adicionar esse componente - por defeito adicionaria uma luz ambiente de (0.2, 0.2, 0.2, 1.0)
    GLfloat lightModelAmbient[] = {0.0, 0.0, 0.0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
    
    // Define o componente difuso da luz 0
    GLfloat diffuseLight[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    
    // Define o componente ambiente da luz 0
    GLfloat ambientLight[] = {0.25, 0.25, 0.25, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    
    // Define o componente especular da luz 0
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    // Activa a luz 0
    glEnable(GL_LIGHT0);
    
    // Activa a iluminacao
    glEnable(GL_LIGHTING);
    
    // Inicializa a semente da funcao de geracao de numeros aleatorios
    srand(time(NULL));
    
    // Inicia o ciclo do glut
    glutMainLoop();
    
    // Como o ciclo do glut e infinito, e so sai quando se toca em Escape (porque assim o definimos), se chegou aqui e porque algo correu mal e retorna-se um valor mau ao SO
    return EXIT_FAILURE;
}
