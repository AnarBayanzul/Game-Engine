#include "SDL_Manager.h"
#include "Shape.h"
#include <SDL_opengl.h>
#include <iostream>

GLuint vert;
GLuint frag;
GLuint program;

int main(int argc, char** argv) {
	// Preloop

	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 500, 500, SDL_TRUE);
	SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);

	// Read mesh file
	std::vector<glm::vec3> data = {};
	Shape cube = Shape(parseMesh("mesh.txt", data), data);


	// Define shaders
	const GLchar * vertShader = R"(
		#version 410
		layout(location = 0) in vec3 pos;
		layout(location = 1) in vec3 norm;
		uniform mat4 proj;
		smooth out vec3 normal;
		void main() {
			normal = norm;
			gl_Position = vec4(pos.xyz + vec3(0.0, 0.0, 2.0), 1.0); // The addition is an offset as we are not using a 'camera' matrix yet...
		}
		)";
	const GLchar * fragShader = R"(
		#version 410
		smooth in vec3 normal;
		out vec4 color;
		void main() {
			vec4 diffuse = vec4(normal.xyz, 1.0f);
			color = diffuse;
		}
		)";
	// Compile shaders
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShader, NULL);

	glCompileShader(vert);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		std::cout << "Vertex Shader:\n";
		std::cout << infoLog;
	}
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShader, NULL);

	glCompileShader(frag);
	// check for shader compile errors
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "Fragment Shader:\n";
		std::cout << infoLog;
	}

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Combined\n";
		std::cout << infoLog;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	glUseProgram(0);
	GLint uniformIndex = glGetUniformLocation(program, "proj");



















	bool exit = false;
	SDL_Event e;
	while (!exit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exit = true;
				break;
			case SDL_EventType::SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Manager::sdl().handleResize(e.window.windowID);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					SDL_Manager::sdl().closeWindow(e.window.windowID);
					break;
				}
				break;
			}
		}
		// Game loop!













		//std::cout << std::hex << e.type << std::endl;
		SDL_Manager::sdl().updateWindows();
	}
	return 0;
}