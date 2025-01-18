#include "SDL_Manager.h"
#include <SDL_opengl.h>
#include <iostream>

int main(int argc, char** argv) {
	SDL_Manager::sdl().spawnWindow("1. Hello SDL", 500, 500, SDL_TRUE);
	
	SDL_Manager::sdl().spawnWindow("2. Second SDL", 200, 200, SDL_FALSE);

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