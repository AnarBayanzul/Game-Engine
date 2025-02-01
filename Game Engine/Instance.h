#pragma once

#include "Mesh.h"

class Instance {
private:
public:
	Mesh* mesh;
	glm::mat4 transform;
	GLint transformUniform;
	GLint normalTransformUniform;
	Instance(Mesh* inputMesh, glm::mat4 inputTransform, GLint tranUI, GLint normTranUI);
	~Instance();
	glm::mat4 normalTransform();
};

