#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>


class Texture {
private:
	glm::vec2 textureCoordinates[3];
	GLuint tbo;
public:
	Texture(std::string fileName, int mipmapLevel);
	~Texture();
};

