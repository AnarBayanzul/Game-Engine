#pragma once
#include <glm/glm.hpp>

struct boneData {
	glm::vec3 origin;
	glm::vec3 parent_to_this;
	int parent;
};