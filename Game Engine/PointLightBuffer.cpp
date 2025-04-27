#include "PointLightBuffer.h"

PointLightBuffer::PointLightBuffer(int width, int height, std::vector<TextureProperties> texProps) : FrameBuffer(width, height, texProps) {
	uniformIndexDiffuse = -1;
	uniformIndexNormal = -1;
	uniformIndexPosition = -1;

	uniformIndexLightPositions = -1;
	uniformIndexLightColors = -1;
}
