#include "TextureManager.hpp"

// Singleton da classe TextureManager
TextureManager *TextureManager::texture_manager_ = 0;

// Unica forma de aceder ao singleton
TextureManager *TextureManager::GetTextureManager(void)
{
    // Se ainda nao tiver sido criado, cria-se o unico objecto TextureManager
	if (texture_manager_ == 0) {
		texture_manager_ = new TextureManager();
	}

	return texture_manager_;
}

void TextureManager::DeleteTextureManager(void)
{
	delete(texture_manager_);
}

GLuint TextureManager::GetTextureName(std::string fileName)
{
	std::string key = fileName;

	std::map<std::string, GLuint>::iterator iterator = textures_.find(key);
	if (iterator != textures_.end()) {
		return iterator->second;
	} else {
		GLuint textureName = Texture::Generate(fileName);
		textures_[key] = textureName;

		return textureName;
	}
}

void TextureManager::DeleteTextures(void)
{
	for (std::map<std::string, GLuint>::iterator iterator = textures_.begin(); iterator != textures_.end(); ++iterator) {
		Texture::Delete(iterator->second);
	}

	textures_.clear();
}

TextureManager::TextureManager(void)
{
  
}

TextureManager::~TextureManager(void)
{
	DeleteTextures();
}
