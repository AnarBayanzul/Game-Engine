#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <Gl/glew.h>

class Shape {
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	GLuint vao;
	GLuint vbo;
public:
	Shape(const size_t triangleCount, const std::vector<glm::vec3>&);
	~Shape();
	GLuint getVAO();
	GLsizei getVertexCount();
};