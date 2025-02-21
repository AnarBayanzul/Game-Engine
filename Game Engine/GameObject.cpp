#include "GameObject.h"

#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

GameObject::GameObject() {
	show = false;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f);
	renderElement = -1;
	textureElement = -1;
	linearV = glm::vec3(0.0f, 0.0f, 0.0f);
	angularV = glm::vec3(0.0f, 0.0f, 0.0f);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

GameObject::GameObject(glm::vec3 pos, quat rot, int RE, int TE, glm::vec3 LV, glm::vec3 AV, glm::vec4 inputColor, bool showToScreen) {
	show = showToScreen;
	position = pos;
	rotation = rot;
	renderElement = RE;
	textureElement = TE;
	linearV = LV;
	angularV = AV;
	color = inputColor;
}

void GameObject::update(float delta) {
	position += linearV * delta;
	if (glm::length(angularV) != 0) {
		rotation = quat(glm::normalize(angularV), delta * glm::length(angularV)) * rotation; // TODO not computationally efficient
	}
}

const glm::mat4 GameObject::getModel() {
	return glm::translate(position) * (glm::mat4) rotation;
}

const int GameObject::getRenderElement() {
	return renderElement;
}

const glm::vec4 GameObject::getColor() {
	return color;
}