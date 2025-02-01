#include "Instance.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"



Instance::Instance(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint normTranUI) {
	mesh = inputMesh;
	transform = inputTransform;
	transformUniform = tranUI;
	normalTransformUniform = normTranUI;
}

Instance::~Instance() {

}

glm::mat4 Instance::normalTransform() {
	return glm::transpose(glm::inverse(transform));
}