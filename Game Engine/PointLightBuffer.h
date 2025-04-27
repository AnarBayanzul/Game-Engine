#pragma once
#include "FrameBuffer.h"
class PointLightBuffer: public FrameBuffer {
private:
public:
	PointLightBuffer(int width, int height, std::vector<TextureProperties> texProps);

	GLint uniformIndexDiffuse;
	GLint uniformIndexNormal;
	GLint uniformIndexPosition;

	GLint uniformIndexLightPositions;
	GLint uniformIndexLightColors;
};

