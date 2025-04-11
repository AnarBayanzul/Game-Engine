#pragma once

#include "GameObject.h"

#define MAXOBJECTSPERTILE 16

struct tile {
	glm::vec3 min;
	glm::vec3 max;
	GameObject* obj[MAXOBJECTSPERTILE]; // might become unwieldy
	int objCount;
};