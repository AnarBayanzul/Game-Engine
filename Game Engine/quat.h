#pragma once
#include <glm/glm.hpp>

class quat {
private:
	// one is the "real" value (cosine of half the angle)
	// three remaining are "imaginary" components (axis vector scaled by sine of half the angle)
	glm::vec4 quaternion;
public:
	quat();
	quat(glm::vec3 axis, float angle);
	quat(glm::vec4 direct); // Directly define elements of quaternion
	~quat();
	quat operator*(quat second);
	glm::vec3 operator*(glm::vec3 point);
	operator glm::mat4();
	quat conjugate();
	glm::vec4 getElements();
};

