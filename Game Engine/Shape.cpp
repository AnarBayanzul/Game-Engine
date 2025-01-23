#include "Shape.h"
#include <string>
#include <iostream>
#include <fstream>

// Returns triangle count, takes in file name and vector for vertices and normals
int parseMesh(std::string filename, std::vector<float>& data) {
	char delimiter = ' ';
	int triangleCount = 0;

	std::fstream input(filename);
	if (input.is_open()) {
		std::string line;
		// triangle count
		std::getline(input, line, delimiter);
		triangleCount = std::stoi(line);
		// Rest of the lines
		//float x;
		//float y;
		//float z;
		for (int i = 0; i < 2 * 3 * 3 * triangleCount; i++) {
			//std::getline(input, line, delimiter);
			//x = std::stof(line);
			//std::getline(input, line, delimiter);
			//y = std::stof(line);
			//std::getline(input, line, delimiter);
			//z = std::stof(line);
			//data.push_back(glm::vec3(x, y, z));
			std::getline(input, line, delimiter);
			data.push_back(std::stof(line));
		}
	}
	return triangleCount;
}

Shape::Shape() {
	std::cout << "default empty constructor of shape is just a triangle\n";
	int triangleCount = 1;
	const std::vector<float>& vertexData = {
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		0.0, 0.5, 0.0,
		
		-0.33, -0.66, 1.0,
		-0.1, -1.0, -0.66,
		-1.0, 0.66, -0.3,
	};
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	
	
	//float read[18];
	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexData.size(), (void*) &read[0]);
	//for (int i = 0; i < 18; i++) {
	//	std::cout << read[i] << std::endl;
	//}
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	GLuint offset = (GLuint)(3 * 3 * triangleCount); // TODO: is this correct?
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (int i = 0; i < 3 * 3 * triangleCount; i = i + 3) {
		pos.push_back(glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]));
		norm.push_back(glm::vec3(vertexData[i + 3 * 3 * triangleCount], vertexData[i + 1 + 3 * 3 * triangleCount], vertexData[i + 2 + 3 * 3 * triangleCount]));
	}
}

Shape::Shape(const size_t triangleCount, const std::vector<float>& vertexData) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	GLuint offset = (GLuint)(3 * 3 * triangleCount); // TODO: is this correct?
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (int i = 0; i < 3 * 3 * triangleCount; i = i + 3) {
		pos.push_back(glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]));
		norm.push_back(glm::vec3(vertexData[i + 3 * 3 * triangleCount], vertexData[i + 1 + 3 * 3 * triangleCount], vertexData[i + 2 + 3 * 3 * triangleCount]));
		//std::cout << vertexData[i] << ',' << vertexData[i + 1] << ',' << vertexData[i + 2] << "        |        ";
		//std::cout << vertexData[i + 3 * 3 * triangleCount] << ',' << vertexData[i + 1 + 3 * 3 * triangleCount] << ',' << vertexData[i + 2 + 3 * 3 * triangleCount] << std::endl;
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
	return pos.size(); // TODO look into integer division in cpp
}
