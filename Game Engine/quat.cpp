#include "quat.h"
#include <glm/glm.hpp>

quat::quat(glm::vec3 inputV, float inputF) {
	quaternion = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

quat quat::operator*(quat inputQuat) {
	return inputQuat;
}

glm::vec3 quat::operator*(glm::vec3) {
	return glm::vec3();
}

quat::operator glm::mat4() {
	return glm::mat4();
}

quat quat::conjugate() {
	return quat(glm::vec3(), 0.0f);
}