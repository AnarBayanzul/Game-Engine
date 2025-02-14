#pragma once
#include <glm/glm.hpp>

class quat {
private:
	glm::vec4 quaternion;
public:
	quat(glm::vec3, float);
	quat operator*(quat);
	glm::vec3 operator*(glm::vec3);
	operator glm::mat4();
	quat conjugate();
};

