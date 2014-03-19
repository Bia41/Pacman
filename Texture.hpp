#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "OpenGL.hpp"
#include <string>
#include "Image.hpp"

// Classe que representa uma textura carregada no OpenGL
namespace Texture
{
	GLuint Generate(std::string fileName);
	void Delete(GLuint name);
}

#endif

