#pragma once
#include "Mesh.h"
#include "quat.h"

#include <SDL_opengl.h>
#include <glm/glm.hpp>

class GameObject {
private:
public:
	Mesh* mesh;
	glm::mat4 transform;
	GLint transformUniform;
	GLint colorUniform;
	glm::vec4 color;
	GameObject(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint colorUI, glm::vec4 inputColor);
	~GameObject();
};

//class GameObject {
//private:
//	glm::vec3 position;
//	quat rotation;
//	int renderElement;
//	glm::vec3 linearV;
//	glm::vec3 angularV;
//public:
//	void update(float delta);
//	glm::mat4 getModel();
//	int getRenderElement();
//};