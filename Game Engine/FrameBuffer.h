#pragma once

#include "Texture.h"

#include <GL/glew.h>
#include <vector>
class FrameBuffer {
private:
	GLuint fbo;
	GLuint* tex;
	GLuint rbo;
	int resX;
	int resY;
public:
	FrameBuffer(int width, int height, std::vector<Texture> textures);
	~FrameBuffer();
	GLuint getFBO();
	GLuint bindFBO();
	void bindTextures();
};

