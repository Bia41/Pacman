#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include "OpenGL.hpp"
#include <map>
#include <string>
#include "Texture.hpp"

// Classe para usar quando se quer uma textura
// Esta classe e um singleton que guarda todas as texturas carregadas no OpenGL de modo a nao carregar duas vezes a mesma
class TextureManager
{
public:
	static TextureManager *GetTextureManager(void);
	static void DeleteTextureManager(void);
	GLuint GetTextureName(std::string fileName);
	void DeleteTextures(void);
	~TextureManager(void);
private:
	TextureManager(void);
	static TextureManager *texture_manager_;
	std::map<std::string, GLuint> textures_;
};

#endif
