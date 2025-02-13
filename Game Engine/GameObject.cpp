#include "GameObject.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"



GameObject::GameObject(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint colorUI, glm::vec4 inputColor) {
	mesh = inputMesh;
	transform = inputTransform;
	transformUniform = tranUI;
	colorUniform = colorUI;
	color = inputColor;
}

GameObject::~GameObject() {

}