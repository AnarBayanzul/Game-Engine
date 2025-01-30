#include "SDL_Manager.h"
#include "Shape.h"
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <iterator>

GLuint program;

// TODO uniformIndex and cube are not supposed to be globals, temp fix
Shape* cube;
GLint uniformIndexProj;
GLint uniformIndexTran;
GLint uniformIndexTranNorm;


int loadShaders(std::string vertFile, std::string fragFile) {
	const GLchar* vertShader;
	const GLchar* fragShader;
	
	GLuint vert;
	GLuint frag;

	std::ifstream inputVert(vertFile);
	std::string vertCode(std::istreambuf_iterator<char>{inputVert}, {});
	vertShader = (GLchar*)vertCode.data();

	std::ifstream inputFrag(fragFile);
	std::string fragCode(std::istreambuf_iterator<char>{inputFrag}, {});
	fragShader = (GLchar*)fragCode.data();


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
	uniformIndexProj = glGetUniformLocation(program, "proj");
	uniformIndexTran = glGetUniformLocation(program, "tran");
	uniformIndexTranNorm = glGetUniformLocation(program, "tranNorm");

	return 0;
}

int main(int argc, char** argv) {
	// Preloop

	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 500, 500, SDL_TRUE);
	//SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);

	// Read mesh file
	std::vector<float> data = {};
	cube = new Shape(parseMesh("monkeyMesh.txt", data, false), data);
	//cube = new Shape(parseMesh("cube.gex", data, true), data);
	//cube = new Shape(parseMesh("torusMesh.txt", data, false), data);
	//cube = new Shape(parseMesh("mesh.txt", data, false), data);
	//cube = new Shape(parseMesh("triangle.txt", data, false), data);



	if (loadShaders("defaultVertexShader.txt", "defaultFragmentShader.txt") != 0) {
		std::cout << "Shaders didn't load correctly\n";
		return 1;
	}




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


	delete cube;
	return 0;
}