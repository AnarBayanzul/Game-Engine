#pragma once

#include "Mesh.h"

class Instance {
private:
public:
	Mesh* mesh;
	glm::mat4 transform;
	GLint transformUniform;
	GLint normalTransformUniform;
	GLint colorUniform;
	glm::vec4 color;
	Instance(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint normTranUI, GLint colorUI, glm::vec4 inputColor);
	~Instance();
	glm::mat4 normalTransform();
};

