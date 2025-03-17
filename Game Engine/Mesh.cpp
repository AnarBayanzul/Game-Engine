#include "Mesh.h"



Mesh::Mesh(const size_t triangleCount, const std::vector<float>& vertexData) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Enable UV vertex attribute
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	void* offsetNormal = (void*)(sizeof(float) * 9 * triangleCount);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offsetNormal);
	void* offsetTexture = (void*)(sizeof(float) * 18 * triangleCount);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, offsetTexture);

	for (int i = 0; i < 9 * triangleCount; i = i + 3) {
		pos.push_back(glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]));
		norm.push_back(glm::vec3(vertexData[i + 9 * triangleCount], vertexData[i + 1 + 9 * triangleCount], vertexData[i + 2 + 9 * triangleCount]));
	} 
	for (int i = 18 * triangleCount; i < 24 * triangleCount; i = i + 2) {
		uv.push_back(glm::vec2(vertexData[i], vertexData[i + 1]));
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
