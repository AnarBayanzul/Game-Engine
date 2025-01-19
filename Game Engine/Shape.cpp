#include "Shape.h"
#include <string>
#include <fstream>

// Returns triangle count, takes in file name and vector for vertices and normals
int parseMesh(std::string filename, std::vector<glm::vec3>& data) {
	char delimiter = ' ';
	int triangleCount = 0;

	std::fstream input(filename);
	if (input.is_open()) {
		std::string line;
		// triangle count
		std::getline(input, line, delimiter);
		triangleCount = std::stoi(line);
		// Rest of the lines
		float x;
		float y;
		float z;
		for (int i = 0; i < 2 * triangleCount; i++) {
			std::getline(input, line, delimiter);
			x = std::stof(line);
			std::getline(input, line, delimiter);
			y = std::stof(line);
			std::getline(input, line, delimiter);
			z = std::stof(line);
			data.push_back(glm::vec3(x, y, z));
		}
	}

	return triangleCount;
}


Shape::Shape(const size_t triangleCount, const std::vector<glm::vec3>& vertexData) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	int offset = 3 * 3 * triangleCount; // TODO: is this correct?
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (int i = 0; i < triangleCount; i++) {
		pos.push_back(vertexData[i]);
		norm.push_back(vertexData[i + triangleCount]);
	}

}

Shape::~Shape() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	vao = 0;
	vbo = 0;
}

GLuint Shape::getVAO() {
	return vao;
}

GLsizei Shape::getVertexCount() {
	return pos.size();
}
