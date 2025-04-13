#include "Camera.h"


#include <glm/ext.hpp>

// Link from chad:
// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf

Camera::Camera(float fovIn, float aspectIn, float closeIn, float farIn) {
	show = false;
	fov = fovIn;
	aspectRatio = aspectIn;
	close = closeIn;
	far = farIn;
	selfType = CAMERA;

	projection = glm::perspective(fov, aspectRatio, close, far);
}

glm::mat4 Camera::getProjection() {
	return projection;
}

void Camera::calculatePlanes() { // TODO should it take into account camera position?
	planes[RIGHT].x = projection[0][3] + projection[0][0];
	planes[RIGHT].y = projection[1][3] + projection[1][0];
	planes[RIGHT].z = projection[2][3] + projection[2][0];
	planes[RIGHT].w = projection[3][3] + projection[3][0];

	planes[LEFT].x = projection[0][3] - projection[0][0];
	planes[LEFT].y = projection[1][3] - projection[1][0];
	planes[LEFT].z = projection[2][3] - projection[2][0];
	planes[LEFT].w = projection[3][3] - projection[3][0];

	planes[TOP].x = projection[0][3] - projection[0][1];
	planes[TOP].y = projection[1][3] - projection[1][1];
	planes[TOP].z = projection[2][3] - projection[2][1];
	planes[TOP].w = projection[3][3] - projection[3][1];

	planes[BOTTOM].x = projection[0][3] + projection[0][1];
	planes[BOTTOM].y = projection[1][3] + projection[1][1];
	planes[BOTTOM].z = projection[2][3] + projection[2][1];
	planes[BOTTOM].w = projection[3][3] + projection[3][1];

	planes[FAR].x = projection[0][2];
	planes[FAR].y = projection[1][2];
	planes[FAR].z = projection[2][2];
	planes[FAR].w = projection[3][2];

	planes[NEAR].x = projection[0][3] - projection[0][2];
	planes[NEAR].y = projection[1][3] - projection[1][2];
	planes[NEAR].z = projection[2][3] - projection[2][2];
	planes[NEAR].w = projection[3][3] - projection[3][2];
}