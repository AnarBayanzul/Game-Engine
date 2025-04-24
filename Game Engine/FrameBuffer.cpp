#include "FrameBuffer.h"

#include <iostream>

FrameBuffer::FrameBuffer(int width, int height, std::vector<TextureProperties> texProps) {
	program = 0;
	resX = width;
	resY = height;
	texCount = texProps.size();
	if (texCount > 8) {
		std::cout << "attaching more than 8 textures to framebuffer, may not work on some hardware.\n";
		if (texCount > 16) {
			std::cout << "attaching more than 16 textures is not supported\n";
		}
	}
	// create FBO
	glGenFramebuffers(1, &fbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "frame buffer not complete\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.01f, 0.01f, 0.06f, 1.0f);
	// ATTACH TO FBO
	tex = new GLuint[texCount];
	colorAttachments = new GLenum[texCount];
	for (int i = 0; i < texCount; ++i) {
		glGenTextures(1, &(tex[i]));
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		glTexImage2D(GL_TEXTURE_2D, texProps[i].level, texProps[i].internalFormat, texProps[i].width, texProps[i].height, texProps[i].border, texProps[i].format, texProps[i].type, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex[i], 0);
		
		colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(texCount, colorAttachments);

	// render buffers can be treated as "write-only"
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "render buffer not attached\n";
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(1, &quadVBO);
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	float quadData[18] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, quadData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &fbo);
	for (int i = 0; i < texCount; ++i) {
		glDeleteTextures(0, &(tex[i]));
	}
	delete[] tex;
	// TODO also delete buffers in VRAM
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

GLuint FrameBuffer::getFBO() {
	return fbo;
}

GLuint FrameBuffer::bindFBO() {
	glBindRenderbuffer(GL_RENDERBUFFER, fbo);
	return 0;
}

GLuint FrameBuffer::getQuadVAO() {
	return quadVAO;
}
void FrameBuffer::setProgram(GLuint programIn) {
	program = programIn;
}

GLuint FrameBuffer::getProgram() {
	return program;
}


void FrameBuffer::bindTextures() {
	// bind all textures in sequence, incrementing from GL_TEXTURE0
	// TODO not incrementing from GL_TEXTURE0 ??
	for (int i = 0; i < texCount; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
	}
}

void FrameBuffer::bindTexture(int i) {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, tex[i]);
}
