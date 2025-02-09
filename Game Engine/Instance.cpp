#include "Instance.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"



Instance::Instance(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint normTranUI, GLint colorUI, glm::vec4 inputColor) {
	mesh = inputMesh;
	transform = inputTransform;
	transformUniform = tranUI;
	normalTransformUniform = normTranUI;
	colorUniform = colorUI;
	color = inputColor;
}

Instance::~Instance() {

}

glm::mat4 Instance::normalTransform() {
	return glm::transpose(glm::inverse(transform));
}