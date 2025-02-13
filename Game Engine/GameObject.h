#pragma once

#include "Mesh.h"

class GameObject {
private:
public:
	Mesh* mesh;
	glm::mat4 transform;
	GLint transformUniform;
	//GLint normalTransformUniform;
	GLint colorUniform;
	glm::vec4 color;
	GameObject(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint colorUI, glm::vec4 inputColor);
	~GameObject();
};

