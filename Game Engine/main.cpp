#include "SDL_Manager.h"
#include "Mesh.h"
#include "GameObject.h"

#include "Utility.h"
#include "Engine.h"

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

int main(int argc, char** argv) {
	// Preloop
	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 400, 400, SDL_TRUE);
	//SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);
	//SDL_Manager::sdl().spawnWindow("3. Third SDL", 200, 200, SDL_FALSE);

	// For time updates
	std::chrono::steady_clock::time_point current;
	std::chrono::steady_clock::time_point previous = std::chrono::steady_clock::now();

	// User intialization
	if (initialize()) {
		std::cout << "Trouble with user initialization\n";
		return 1;
	}

	bool exit = false;
	SDL_Event e;
	while (!exit) {
		// Calculate time delta
		current = std::chrono::steady_clock::now();
		deltaSec = (current - previous).count() / 1000000000.0f;
		delta = (size_t) (deltaSec * 1000);
		previous = current;
		//std::cout << deltaSec << std::endl;
		//std::cout << delta << std::endl << std::endl;

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
			case SDL_KEYDOWN:
				lastKey = e.key;
				break;
			}
		}

		// Game loop!
		globalUpdate(deltaSec);
		std::this_thread::sleep_for(1ms);


		//std::cout << std::hex << e.type << std::endl;
		SDL_Manager::sdl().updateWindows();
	}

	// Deallocate from heap
	for (int i = 0; i < renderCount; ++i) {
		delete RenderQueue[i];
	}

	return 0;
}