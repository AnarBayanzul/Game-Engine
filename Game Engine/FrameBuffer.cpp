#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height, std::vector<Texture> textures) {
	resX = width;
	resY = height;
	// create FBO

	// When creating texture buffers, you don't need to buffer
	// last argument of glTexImage2D can be NULL or 0

	// consider throwing complaint if attaching more than 8 textures to an FBO


	// HOW TO ATTACH TO FBO
		// Gen tex buffer
		// bind tex buffer
		// set tex properties with glTexImage2D
		// set additional tex parameters
			// primarily GL_TEXTURE_MIN_FILTER and GL_TEXTURE_MAG_FILTER
		// Attach texture to framebuffer, ensure correct indexing of attachments
			// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT_0 + index, GL_TEXTURE_2D, textureBufferCreated, 0);
			// You will need to start index at 0, and increment it in a loop for each texture reated and attached to the FBO
			// fourth argument is the name/ID generated for the texture being attached
			// last argument is level in texture to attach to the FBO
				// level refers to mipmaps, 0 is full texture
	// Loop this for the number of textures to create and attach
	// textures should all have the same resolution
}

FrameBuffer::~FrameBuffer() {
	delete[] tex;
	// TODO also delete buffers in VRAM
}

GLuint FrameBuffer::getFBO() {
	return fbo;
}

GLuint FrameBuffer::bindFBO() {
	return 0;
}

void FrameBuffer::bindTextures() {
	// bind all textures in sequence, incrementing from GL_TEXTURE0
}