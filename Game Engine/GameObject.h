#pragma once
#include "Mesh.h"
#include "quat.h"

#include <SDL_opengl.h>
#include <glm/glm.hpp>

class GameObject {
private:
	glm::vec3 position;
	quat rotation = {glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)}; // TODO change?
	int renderElement; // mesh element
	int textureElement;
	glm::vec3 linearV;
	glm::vec3 angularV;
	glm::vec4 color;
public:
	bool show;
	GameObject(glm::vec3 pos, quat rot, int RE, int TE, glm::vec3 LV, glm::vec3 AV, glm::vec4 inputColor, bool showToScreen);
	GameObject();
	virtual void update(float delta);
	const glm::mat4 getModel();
	const int getRenderElement();
	const int getTextureElement();
	const glm::vec4 getColor();
};