#include "Image.hpp"

#include <iostream>

// Tamanho do cabecalho do ficheiro de imagem .tga
static const int TARGA_IMAGE_HEADER_SIZE = 18;

static const int BITS_PER_BYTE = 8;

Image::Image(const std::string fileName)
{
    // Abre o ficheiro com o nome dado para ler em formato binario
	std::ifstream file(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
    
    // Array para guardar a informacao do cabecalho
	unsigned char imageHeaderBuffer[TARGA_IMAGE_HEADER_SIZE];
    
    // Le para o array o cabecalho do ficheiro
	file.read(reinterpret_cast<char *>(&imageHeaderBuffer[0]), TARGA_IMAGE_HEADER_SIZE);

    // Guarda os valores de largura, altura e bits por pixel da imagem (Ver formato TARGA)
	width_  = (imageHeaderBuffer[13] * 256) + imageHeaderBuffer[12];
	height_ = (imageHeaderBuffer[15] * 256) + imageHeaderBuffer[14];
	bits_per_pixel_ = imageHeaderBuffer[16];
    
    // Ve a quantidade de bits por pixel para saber se tem um canal de transparencia ou nao
	has_alpha_ = (bits_per_pixel_ == 32) ? true : false;

	unsigned int numberOfBytes = (bits_per_pixel_ / BITS_PER_BYTE) * width_ * height_;

    // Le os dados da imagem que seguem o cabecalho
	data_ = new unsigned char[numberOfBytes];
	file.read(reinterpret_cast<char *>(data_), numberOfBytes);
    
    // Fecha o ficheiro
	file.close();

    // Troca a ordem dos bytes para serem do agrado do OpenGL, que espera na forma RGB(A) e estes estao na forma BGR(A)
	for (unsigned int bgrSwap = 0; bgrSwap < numberOfBytes; bgrSwap += (bits_per_pixel_ / BITS_PER_BYTE)) {
		unsigned char temporaryByte = data_[bgrSwap];
		data_[bgrSwap] = data_[bgrSwap + 2];
		data_[bgrSwap + 2] = temporaryByte;
	}
}

Image::~Image(void)
{
	delete[] data_;
}
