#pragma once

#include <GL/glew.h>

// stores general information needed to create a texture in OpenGL
struct TextureProperties {
	GLint level;
	GLint internalFormat;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;
};