#pragma once

#include "Mesh.h"

class Instance {
private:
public:
	Mesh* mesh;
	glm::mat4 transform;
	Instance(Mesh* inputMesh, glm::mat4 inputTransform);
	~Instance();
	glm::mat4 normalTransform();
};

