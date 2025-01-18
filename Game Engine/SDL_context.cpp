#include "SDL_Manager.h"

int main(int argc, char** argv) {

	SDL_Manager::sdl().spawnWindow("Hello SDL", 500, 500, (SDL_bool) true);
	
	SDL_Manager::sdl().spawnWindow("Second SDL", 200, 200, (SDL_bool) false);

	
	bool exit = false;
	SDL_Event e;
	while (!exit) {
		// Game loop!



		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				exit = true;
				break;
				// TODO: check if below is working
			case SDL_EventType::SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Manager::sdl().handleResize(e.window.windowID);
					// std::cout << "window resized\n";
					break;
				case SDL_WINDOWEVENT_CLOSE:
					SDL_Manager::sdl().closeWindow(e.window.windowID);
					break;
				}
				break;
			}
		}
		SDL_Manager::sdl().updateWindows();
	}
	
	
	return 0;
}