#pragma once
#include "Keyframe.h"

#include <vector>
#include <string>

class Animation {
private:


public:
	int keyCount;
	int boneCount;
	std::vector<Keyframe> animData;
	Animation(int keyIn, int boneIn, const std::vector<Keyframe>& data);
	~Animation();
};

