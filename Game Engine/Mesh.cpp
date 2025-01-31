#include "Mesh.h"
#include <string>
#include <iostream>
#include <fstream>

// Returns triangle count, takes in file name and vector for vertices and normals
int parseMesh(std::string filename, std::vector<float>& data, bool bytes) {
	int triangleCount = 0;
	std::fstream input(filename);
	if (bytes) {
		float value;
		if (input.is_open()) {
			// triangle count
			input.read((char*)&triangleCount, 4);
			for (int i = 0; i < 2 * 3 * 3 * triangleCount; i++) {
				input.read((char*)&value, 4);
				data.push_back(value);
			}
		}
	} else {
		char delimiter = ' ';
		if (input.is_open()) {
			std::string line;
			// triangle count
			std::getline(input, line, delimiter);
			triangleCount = std::stoi(line);
			// Rest of the lines
			for (int i = 0; i < 2 * 3 * 3 * triangleCount; i++) {
				std::getline(input, line, delimiter);
				data.push_back(std::stof(line));
			}
		}
	}
	return triangleCount;
}

Mesh::Mesh(const size_t triangleCount, const std::vector<float>& vertexData) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	GLuint offset = (GLuint)(sizeof(float) * 3 * 3 * triangleCount);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (int i = 0; i < 3 * 3 * triangleCount; i = i + 3) {
		pos.push_back(glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]));
		norm.push_back(glm::vec3(vertexData[i + 3 * 3 * triangleCount], vertexData[i + 1 + 3 * 3 * triangleCount], vertexData[i + 2 + 3 * 3 * triangleCount]));
	}

}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	vao = 0;
	vbo = 0;
}

GLuint Mesh::getVAO() {
	return vao;
}

GLsizei Mesh::getVertexCount() {
	return pos.size();
}
