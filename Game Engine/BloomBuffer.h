#pragma once
#include "FrameBuffer.h"
class BloomBuffer : public FrameBuffer {
private:

public:
    BloomBuffer(int width, int height, std::vector<TextureProperties> texProps);

    GLint uniformIndexDiffuse;
    GLint uniformIndexPosition;
    GLint uniformIndexBloom;
    GLint uniformIndexVisibility;
};

