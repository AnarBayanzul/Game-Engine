#include "GameObject.h"

#include "Mesh.h"
#include "Engine.h"

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
	selfType = GAMEOBJECT;
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
	selfType = GAMEOBJECT;
}

void GameObject::update(float delta) {
	position += integrateLinear(delta, linearV);
	quat updateRot = integrateAngular(delta, angularV);
	rotation = updateRot * rotation;
	//rotation = quat(glm::vec3(0.0, 1.0, 0.0), 0.0);
}


// TODO for integration
void GameObject::integrateAcceleration(float deltaTime, const glm::vec3& accel) {
	// velocity updated by integration of linear acceleration
	linearV += integrateLinear(deltaTime, accel);
}

void GameObject::integrateAngularAcceleration(float deltaTime, const glm::vec3& accel) {
	// angular velocity updated by integration of angular acceleration
	// could use integrateLinear
	angularV += integrateLinear(deltaTime, accel);
}

void GameObject::applyLinearImpulse(const glm::vec3& impulse) {
	// apply a change in velocity directly
	linearV += impulse;
}

void GameObject::applyAngularImpulse(const glm::vec3& impulse) {
	// apply a change in angular velocity directly
	angularV += impulse;
}





const glm::mat4 GameObject::getModel() {
	return glm::translate(position) * (glm::mat4) rotation;
	//return glm::translate((glm::mat4)rotation, position);
}
  
const int GameObject::getRenderElement() {
	return renderElement;
}

const int GameObject::getTextureElement() {
	return textureElement;
}

const glm::vec4 GameObject::getColor() {
	return color;
}

void GameObject::move(glm::vec3 newP) { // offset position
	position += newP;
}

void GameObject::setPosition(glm::vec3 newP) {
	position = newP;
}

glm::vec3 GameObject::getPosition() {
	return position;
}

quat GameObject::getRotation() {
	return rotation;
}

void GameObject::setTextureElement(int TE) {
	textureElement = TE;
}

void GameObject::setRenderElement(int RE) {
	renderElement = RE;
}
void GameObject::setRotation(quat rotationIn) {
	rotation = rotationIn;
}

objectType GameObject::getType() {
	return selfType;
}

glm::vec3 GameObject::getAABBmin() {
	return AABBmin;
}

glm::vec3 GameObject::getAABBmax() {
	return AABBmax;
}

void GameObject::setAABBmin(glm::vec3 newP) {
	AABBmin = newP;
}

void GameObject::setAABBmax(glm::vec3 newP) {
	AABBmax = newP;
}

glm::mat4 GameObject::getParentTransform() {
	return parentTransform;
}
void GameObject::setParentTransform(glm::mat4 newM) {
	parentTransform = newM;
}