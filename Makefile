UNAME := $(shell uname)
CC = g++
CFLAGS = -ansi -pedantic -Wall

ifeq ($(UNAME),Darwin)
LFLAGS = -framework OpenGL -framework GLUT
endif

ifeq ($(UNAME),Linux)
LFLAGS = -lGL -lGLU -lglut
endif

SOURCES = main.cpp Maze.cpp Pacman.cpp Vector.cpp Eyes.cpp Eyebrows.cpp Ghost.cpp Utilities.cpp Texture.cpp TextureManager.cpp Image.cpp ParticleManager.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = pacman

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
