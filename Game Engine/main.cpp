#include "SDL_Manager.h"
#include "Shape.h"
#include <SDL_opengl.h>
#include <iostream>

GLuint vert;
GLuint frag;
GLuint program;

// TODO uniformIndex and cube are not supposed to be globals, temp fix
Shape* cube;
GLint uniformIndex;
GLint uniformIndex2;


int main(int argc, char** argv) {
	// Preloop

	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 500, 500, SDL_TRUE);
	//SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);

	// Read mesh file
	std::vector<float> data = {};
	//cube = new Shape(parseMesh("monkeyMesh.txt", data), data);
	cube = new Shape(parseMesh("mesh.txt", data), data);
	//cube = new Shape(parseMesh("triangle.txt", data), data);
	//cube = new Shape(); // TODO free cube


	// Define shaders
	const GLchar * vertShader = R"(
		#version 410
		layout(location = 0) in vec3 pos;
		layout(location = 1) in vec3 norm;
		uniform mat4 proj;
		uniform mat4 tran;
		smooth out vec3 normal;
		void main() {
			normal = (tran * vec4(norm.xyz, 0.0)).xyz;
			//normal = norm;
			gl_Position = proj * (tran * vec4(pos.xyz, 1.0) + vec4(0.0, 0.0, -5.0, 0.0)); // The addition is an offset as we are not using a 'camera' matrix yet...
			//gl_Position = vec4(pos.xyz + vec3(0.0, 0.0, 2.0), 1.0);
		}
		)";
	const GLchar * fragShader = R"(
		#version 410
		smooth in vec3 normal;
		out vec4 color;
		//out vec4 FragColor;

		void main() {

			vec3 lightDir = vec3(0.0f, 0.707f, 0.707f);
			vec4 matCol = vec4(0.1f, 0.8f, 0.8f, 1.0f);

			float intensity = 1 - dot(lightDir.xyz, normal.xyz);

			vec4 diffuse = intensity * vec4(matCol.xyz, 1.0f);
			color = diffuse;
			//color = vec4(normal.xyz, 1.0f);
			//FragColor = matCol;
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
	uniformIndex = glGetUniformLocation(program, "proj");
	uniformIndex2 = glGetUniformLocation(program, "tran");











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