#pragma once

#include <glm/glm.hpp>

struct simplex {
	int dimensions;
	int newest;
	float squareShortest;
	glm::vec3 points[4];
};