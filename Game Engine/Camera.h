#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>

enum planes {
	LEFT = 0,
	RIGHT,
	BOTTOM,
	TOP,
	NEAR,
	FAR
};

class Camera: public GameObject {
private:
	float fov; // TODO these may be unnecessary
	float aspectRatio;
	float close;
	float far;
	glm::mat4 projection; // TODO remove render perspective attribute

public:
	Camera(float fovIn, float aspectIn, float closeIn, float farIn);
	glm::mat4 getProjection();


	glm::vec4 planes[6];
	void calculatePlanes();
};
