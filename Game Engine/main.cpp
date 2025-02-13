#include "SDL_Manager.h"
#include "Mesh.h"
#include "GameObject.h"
#include <SDL_opengl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <chrono>

#include "Utility.h"
#include "Engine.h"


int main(int argc, char** argv) {
	// Preloop
	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 400, 400, SDL_TRUE);
	//SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);
	//SDL_Manager::sdl().spawnWindow("3. Third SDL", 200, 200, SDL_FALSE);

	if (loadShaders("defaultVertexShader.txt", "defaultFragmentShader.txt")) {
	//if (loadShaders("defaultVertexShader.txt", "lambertianFragmentShader.txt") != 0) {
	//if (loadShaders("defaultVertexShader.txt", "celShadingFragment.txt") != 0) {
		std::cout << "Shaders didn't load correctly\n";
		return 1;
	}

	// For time updates
	std::chrono::steady_clock::time_point current;
	std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();







	// Read mesh file
	std::vector<float> data = {};
	//cubeMesh = new Mesh(parseMesh("monkeyMesh.txt", data, false), data);
	cubeMesh = new Mesh(parseMesh("monkeySmoothNormal.txt", data, false), data);
	//cube = new Mesh(parseMesh("cube.gex", data, true), data);
	//cube = new Mesh(parseMesh("mesh.txt", data, false), data);
	cube = new GameObject(cubeMesh, glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f)), -3.1415f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)), uniformIndexTran, uniformIndexColor, glm::vec4(0.3f, 0.5f, 0.5f, 1.0f));


	// User intialization
	initialize();


	bool exit = false;
	SDL_Event e;
	while (!exit) {
		// Calculate time delta
		current = std::chrono::steady_clock::now();
		delta = std::chrono::duration_cast<std::chrono::milliseconds>(current - previous);
		deltaSec = std::chrono::duration_cast<std::chrono::seconds>(delta);
		previous = current;

		// Poll events
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




		// Rotate cube
		glBindVertexArray(cube->mesh->getVAO());
		glUseProgram(program);
		cube->transform = glm::rotate(cube->transform, 0.005f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(cube->transformUniform, 1, GL_FALSE, glm::value_ptr(cube->transform));
		glUniform4fv(cube->colorUniform, 1, glm::value_ptr(cube->color));
		glBindVertexArray(0);
		glUseProgram(0);






		//std::cout << std::hex << e.type << std::endl;
		SDL_Manager::sdl().updateWindows();
	}

	// Deallocate from heap
	delete cubeMesh;
	delete cube;
	return 0;
}