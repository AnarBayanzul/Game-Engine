#pragma once

#include "quat.h"
#include "vertexBone.h"
#include "boneData.h"

#include <Gl/glew.h>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> uv;
	GLuint vao;
	GLuint vbo;

	// ---x---y---z---
	// 0 min min min
	// 1 min min max
	// 2 min max min
	// 3 min max max
	// 4 max min min
	// 5 max min max
	// 6 max max min
	// 7 max max max
	glm::vec3 boundaryPoints[8];
public:
	// bone data
	std::vector<vertexBone> boneWeights;
	std::vector<boneData> bones;

	Mesh(const size_t triangleCount, const std::vector<float>& data, const std::vector<vertexBone>& vertexBoneData, const std::vector<boneData>& bonesIn, bool boned);
	~Mesh();
	const GLuint getVAO();
	const GLsizei getVertexCount();
	const glm::vec3* getBoundaryPoints();
	glm::vec3 findSupport(glm::vec3 direction, quat rotation, glm::vec3 origin);
};