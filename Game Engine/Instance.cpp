#include "Instance.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"



Instance::Instance(Mesh* inputMesh, glm::mat4 inputTransform) {
	mesh = inputMesh;
	transform = inputTransform;
}

Instance::~Instance() {

}

glm::mat4 Instance::normalTransform() {
	return glm::transpose(glm::inverse(transform));
}