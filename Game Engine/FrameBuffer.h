#pragma once

#include "Texture.h"
#include "Engine.h"
#include "TextureProperties.h"

#include <GL/glew.h>
#include <vector>
class FrameBuffer {
private:
	GLuint program;
	GLuint fbo;
	GLuint* tex;
	GLenum* colorAttachments;
	int texCount;
	GLuint rbo;
	int resX;
	int resY;

	GLuint quadVAO;
	GLuint quadVBO;
public:
	FrameBuffer(int width, int height, std::vector<TextureProperties> texProps);
	~FrameBuffer();
	GLuint getFBO();
	GLuint bindFBO();
	GLuint getQuadVAO();
	void setProgram(GLuint programIn);
	GLuint getProgram();
	void bindTextures();
	void bindTexture(int i);
};

