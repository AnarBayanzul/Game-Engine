#include "Engine.h"

#include <iostream>
#include <thread>
// This is source file for end user's application layer
#include "quat.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void initialize() {
	std::cout << "user successfully performs initialization\n";

	quat example = { glm::vec3(0.0f, 0.0f, 1.0f), 3.141592654f / 2.0f };
	glm::vec3 point = { 2.0, 0.0, 3.0 };

	glm::vec3 rotated = example * point;
	std::cout << "Point rotation: " << rotated.x << "," << rotated.y << "," << rotated.z << std::endl;

	std::cout << "to mat4: " << glm::to_string((glm::mat4)example) << std::endl;

}