#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <Gl/glew.h>

int parseMesh(std::string filename, std::vector<float>& data);

class Shape {
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	GLuint vao;
	GLuint vbo;
public:
	Shape();
	Shape(const size_t triangleCount, const std::vector<float>& data);
	~Shape();
	GLuint getVAO();
	GLsizei getVertexCount();
};