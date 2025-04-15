#pragma once

#include "quat.h"

#include <vector>

class Keyframe { // glorified struct
public:
	float timeStamp;
	std::vector<std::pair<int, quat>> boneTransform;
	Keyframe();
	Keyframe(float timeIn, std::vector<int> boneID, std::vector<quat> quatIn);
	~Keyframe();
};

