#include "Texture.h"

#include "Utility.h"

#include <SDL_surface.h>

Texture::Texture(std::string fileName, int mipmapLevel) {
	SDL_Surface* data = parseTexture(fileName);

	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_2D, tbo); // todo this might be unneccessary

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, data->w, data->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data->pixels); // TODO ,might be ABGR, instead of RGBA
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(data);
}

Texture::~Texture() {

}

GLuint Texture::getTBO() {
	return tbo;
}