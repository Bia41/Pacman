#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <fstream>

// Classe que representa uma imagem
// Por enquanto ainda so consegue abrir ficheiros de imagem no formato .tga sem compressao, mas aceitando com ou sem canal de transparencia
class Image
{
public:
	Image(const std::string fileName);
	~Image(void);
  
	unsigned int width(void) const;
	unsigned int height(void) const;
	unsigned int bits_per_pixel(void) const;
	bool has_alpha(void) const;
	unsigned char *data(void) const;
  
private:
	unsigned int width_;
	unsigned int height_;
	unsigned int bits_per_pixel_;
	bool has_alpha_;
	unsigned char *data_;
};

inline unsigned int Image::width(void) const
{
	return width_;
}

inline unsigned int Image::height(void) const
{
	return height_;
}

inline unsigned int Image::bits_per_pixel(void) const
{
	return bits_per_pixel_;
}

inline bool Image::has_alpha(void) const
{
	return has_alpha_;
}

inline unsigned char *Image::data(void) const
{
	return data_;
}

#endif
