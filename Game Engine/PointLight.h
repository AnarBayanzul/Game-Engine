#pragma once
#include "GameObject.h"
class PointLight: public GameObject {
private:

public:
	PointLight(glm::vec3 pos, glm::vec4 colorIn);
};

