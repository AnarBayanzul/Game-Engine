#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Gl/glew.h>

int parseMesh(std::string filename, std::vector<glm::vec3>& data);

class Shape {
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	GLuint vao;
	GLuint vbo;
public:
	Shape(const size_t triangleCount, const std::vector<glm::vec3>& data);
	~Shape();
	GLuint getVAO();
	GLsizei getVertexCount();
};