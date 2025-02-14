#pragma once

#include <Gl/glew.h>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
private:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	GLuint vao;
	GLuint vbo;
public:
	Mesh(const size_t triangleCount, const std::vector<float>& data);
	~Mesh();
	GLuint getVAO();
	GLsizei getVertexCount();
};