#include "SDL_Manager.h"


SDL_Manager::SDL_Manager() {
	count = 0;
	SDL_Init(SDL_INIT_VIDEO);
}

SDL_Manager::~SDL_Manager() {
	// TODO:Delete all remaining windows
	while (count != 0) {
		SDL_Manager::closeWindow(0);
	}
	SDL_Quit();
}

SDL_Manager& SDL_Manager::sdl() {
	static SDL_Manager instance;
	return instance;
}

void SDL_Manager::closeWindow(uint32_t id) {
	if (count == 0) {
		// IDK if this is even possible
		std::cout << "Close window when no windows?\n";
		return;
	}
	// iterate over windows, using SDL_GetWindowID to find window
	for (int i = 0; i < count; i++) {
		if (SDL_GetWindowID(windows[i]) == id) {
			// Swap
			std::swap(buffers[i], buffers[count - 1]);
			std::swap(windows[i], windows[count - 1]);
			// n pop
			SDL_DestroyWindow(windows[count - 1]);
			// TODO: do i need to destroy buffer?
			windows[count - 1] = nullptr;
			count--;
		}
	}
}

void SDL_Manager::spawnWindow(const char* title, int width, int height, SDL_bool resizable) {
	if (count >= MAXWINDOWS) {
		// Max windows reached
		std::cout << "max windows already reached\n";
		return;
	}
	windows[count] = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
	if (windows[count] == NULL) {
		// error
		// TODO: cleanup and shutdown with grace
		std::cout << "window creation unsuccessful\n";
		return;
	}
	SDL_SetWindowResizable(windows[count], resizable);

	buffers[count] = SDL_GetWindowSurface(windows[count]);
	SDL_FillRect(buffers[count], NULL, SDL_MapRGB((buffers[count])->format, 0xFF, 0x00, 0xFF));


	count++;



}

void SDL_Manager::handleResize(uint32_t id) {
	for (int i = 0; i < count; i++) {
		if (SDL_GetWindowID(windows[i]) == id) {
			buffers[i] = SDL_GetWindowSurface(windows[i]);
		}
	}
}


void SDL_Manager::updateWindows() {
	for (size_t i = 0; i < count; i++) {
		SDL_UpdateWindowSurface(windows[i]);
		// TODO: later apply special case for when applying OpenGL rendering context
	}
}