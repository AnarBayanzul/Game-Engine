#include "BloomBuffer.h"

BloomBuffer::BloomBuffer(int width, int height, std::vector<TextureProperties> texProps) : FrameBuffer(width, height, texProps) {
	uniformIndexBloom = -1;
	uniformIndexDiffuse = -1;
	uniformIndexPosition = -1;
	uniformIndexVisibility = -1;
}
