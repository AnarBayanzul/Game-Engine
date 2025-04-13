#pragma once
#include "Mesh.h"
#include "quat.h"

#include <SDL_opengl.h>
#include <glm/glm.hpp>


enum objectType {
	GAMEOBJECT,
	CAMERA,
	// insert more types here
	NUMOFTYPES
};

class GameObject {
protected:
	objectType selfType;
private:
	glm::vec3 position;
	quat rotation = {glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)};
	// TODO, in an ideal world the var below shouldn't exist
	glm::mat4 parentTransform; // Not guaranteed to be up to date
	int renderElement; // mesh element
	int textureElement;
	glm::vec3 linearV;
	glm::vec3 angularV;
	glm::vec4 color;


	// Does not account for parent transformation
	glm::vec3 AABBmin;
	// Does not account for parent transformation
	glm::vec3 AABBmax;
public:
	bool show;
	GameObject(glm::vec3 pos, quat rot, int RE, int TE, glm::vec3 LV, glm::vec3 AV, glm::vec4 inputColor, bool showToScreen);
	GameObject();
	void move(glm::vec3 newP); // offset position
	void setPosition(glm::vec3 newP);
	glm::vec3 getPosition();
	void setRotation(quat rotationIn);
	quat getRotation();
	void setTextureElement(int TE);
	const int getRenderElement();
	void setRenderElement(int RE);
	const int getTextureElement();
	virtual void update(float delta);
	const glm::mat4 getModel();
	const glm::vec4 getColor();

	glm::mat4 getParentTransform();
	void setParentTransform(glm::mat4 newM);

	objectType getType();

	glm::vec3 getAABBmin();
	glm::vec3 getAABBmax();
	void setAABBmin(glm::vec3 newP);
	void setAABBmax(glm::vec3 newP);


	// TODO might need to change return types.
	void integrateAcceleration(float deltaTime, const glm::vec3& accel);
	void integrateAngularAcceleration(float deltaTime, const glm::vec3& accel);
	void applyLinearImpulse(const glm::vec3& impulse);
	void applyAngularImpulse(const glm::vec3& impulse);
};