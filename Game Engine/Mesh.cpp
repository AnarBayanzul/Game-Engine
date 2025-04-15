#include "Mesh.h"

#include <iostream>
Mesh::Mesh(const size_t triangleCount, const std::vector<float>& vertexData, const std::vector<vertexBone>& vertexBoneData, const std::vector<boneData>& bonesIn, bool boned) {
	// combine vertexData and vertexBone data
	std::vector<float> allData = vertexData;
	for (int i = 0; i < vertexBoneData.size(); ++i) {
		for (int j = 0; j < 4; ++j) {
			allData.push_back((float)vertexBoneData[i].index[j]);
		}
	}
	for (int i = 0; i < vertexBoneData.size(); ++i) {
		for (int j = 0; j < 4; ++j) {
			allData.push_back(vertexBoneData[i].weight[j]);
		}
	}
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * allData.size(), &allData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Enable UV vertex attribute
	glEnableVertexAttribArray(2);
	// bone ID
	glEnableVertexAttribArray(3);
	// bone weight
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	void* offsetNormal = (void*)(sizeof(float) * 9 * triangleCount);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offsetNormal);
	void* offsetTexture = (void*)(sizeof(float) * 18 * triangleCount);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, offsetTexture);

	// vertex bone attributes
	void* offsetUV = (void*)(sizeof(float) * 24 * triangleCount);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, offsetUV);
	void* offsetID = (void*)(sizeof(float) * (36 * triangleCount));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, offsetID);
	//void* offsetWeights = (void*)(sizeof(float) * 48 * triangleCount);



	float minMax[6] = { vertexData[0], vertexData[1], vertexData[2], vertexData[0], vertexData[1], vertexData[2] }; // min xyz, then max xyz

	for (int i = 18 * triangleCount; i < 24 * triangleCount; i = i + 2) {
		uv.push_back(glm::vec2(vertexData[i], vertexData[i + 1]));
	}
	for (int i = 0; i < 9 * triangleCount; i = i + 3) {
		pos.push_back(glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]));
		norm.push_back(glm::vec3(vertexData[i + 9 * triangleCount], vertexData[i + 1 + 9 * triangleCount], vertexData[i + 2 + 9 * triangleCount]));

		// go to last pushed pos element
		// check if should be added to min and max
		for (int j = 0; j < 3; ++j) {
			minMax[j] = std::min(minMax[j], vertexData[i + j]);
			minMax[j + 3] = std::max(minMax[j + 3], vertexData[i + j]);
		}
	} 

	// generate boundary points from min and max
	for (int i = 0; i < 8; ++i) {
		boundaryPoints[i] = glm::vec3(
			minMax[((i & 0b100) >> 2) * 3 + 2],
			minMax[((i & 0b010) >> 1) * 3 + 1],
			minMax[(i & 0b001) * 3]
		);
	}


	if (boned) {
		boneWeights = vertexBoneData;
		bones = bonesIn;
	}
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	vao = 0;
	vbo = 0;
}

const GLuint Mesh::getVAO() {
	return vao;
}

const GLsizei Mesh::getVertexCount() {
	return (GLsizei) pos.size();
}

const glm::vec3* Mesh::getBoundaryPoints() {
	return boundaryPoints;
}

glm::vec3 Mesh::findSupport(glm::vec3 direction, quat rotation, glm::vec3 origin) {
	// TODO could be done better by inverting rotatoin and applying to search direction
	glm::vec3 rotatedDir = rotation.conjugate() * direction;
	glm::vec3 furthest = pos[0];
	float max = glm::dot(pos[0], rotatedDir);
	float result;
	for (int i = 0; i < pos.size(); ++i) {
		result = glm::dot(pos[i], rotatedDir);
		if (result > max) {
			max = result;
			furthest = pos[i];
		}
	}
	// this was local space
	return (rotation * furthest) + origin; // now global space
}
