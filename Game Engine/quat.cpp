#include "quat.h"
#include <glm/glm.hpp>
#include <iostream>

quat::quat(glm::vec3 axis, float angle) {
	// real part should be cos half angle
	float halfAngle = angle * 0.5f; // TODO can you do bitwise operation in float to divide by two?
	float cosA = cos(halfAngle);
	float sinA = sin(halfAngle);

	quaternion = glm::vec4(cosA, sinA * axis.x, sinA * axis.y, sinA * axis.z);
	//std::cout << "Square magnitude: " << glm::dot(quaternion, quaternion) << std::endl;
	//quaternion = glm::normalize(quaternion); // normalize line might not be needed
}

quat::quat(glm::vec4 direct) {
	quaternion = direct;
}
quat::quat() { // no rotation
	quaternion = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

quat::~quat() {

}



// Hamilton product
quat quat::operator*(quat second) {
	return quat(glm::vec4( // TODO will this disappear out of scope
		glm::dot(quaternion, second.conjugate().quaternion),
		quaternion.x * second.quaternion.y + quaternion.y * second.quaternion.x + quaternion.z * second.quaternion.w - quaternion.w * second.quaternion.z,
		quaternion.x * second.quaternion.z - quaternion.y * second.quaternion.w + quaternion.z * second.quaternion.x + quaternion.w * second.quaternion.y,
		quaternion.x * second.quaternion.w + quaternion.y * second.quaternion.z - quaternion.z * second.quaternion.y + quaternion.w * second.quaternion.x
	));
}

// Rotation
glm::vec3 quat::operator*(glm::vec3 point) {
	// p = q * p * q^-1
	glm::vec4 product = ((*this * quat(glm::vec4(0, point))) * conjugate()).quaternion;
	//std::cout << "Should be 0: " << product.x << std::endl;
	return glm::vec3(product.y, product.z, product.w);
}

quat::operator glm::mat4() {
	return glm::mat4(
		glm::vec4(quaternion.x, quaternion.w, -quaternion.z, quaternion.y),
		glm::vec4(-quaternion.w, quaternion.x, quaternion. y, quaternion.z),
		glm::vec4(quaternion.z, -quaternion.y, quaternion.x, quaternion.w),
		glm::vec4(-quaternion.y, -quaternion.z, -quaternion.w, quaternion.x)
	) * glm::mat4(
		glm::vec4(quaternion.x, quaternion.w, -quaternion.z, -quaternion.y),
		glm::vec4(-quaternion.w, quaternion.x, quaternion.y, -quaternion.z),
		glm::vec4(quaternion.z, -quaternion.y, quaternion.x, -quaternion.w),
		glm::vec4(quaternion.y, quaternion.z, quaternion.w, quaternion.x)
	);
}

quat quat::conjugate() {
	return quat(glm::vec4( // TODO will this disappear out of scope?
		quaternion.x,
		-quaternion.y,
		-quaternion.z,
		-quaternion.w
	));
}


glm::vec4 quat::getElements() {
	return quaternion;
}