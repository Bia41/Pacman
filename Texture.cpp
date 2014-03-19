#include "Texture.hpp"

GLuint Texture::Generate(std::string fileName)
{
    // Cria um objecto de Imagem a partir do nome do ficheiro
	Image *image = new Image(fileName);

    // Gera um handle de textura novo
	GLuint textureName;
	glGenTextures(1, &textureName);
    
    // Faz essa textura ser a activa
	glBindTexture(GL_TEXTURE_2D, textureName);

    // Define os parametros de minimizacao e maximizacao da textura para usar interpolacao linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cria a textura com os dados da imagem
	glTexImage2D(GL_TEXTURE_2D,
				0,
				(image->has_alpha() == true) ? GL_RGBA : GL_RGB,
				image->width(),
				image->height(),
				0,
				(image->has_alpha() == true) ? GL_RGBA : GL_RGB,
				GL_UNSIGNED_BYTE,
				image->data());

	delete image;

	return textureName;
}

void Texture::Delete(GLuint name)
{
	glDeleteTextures(1, &name);
}
