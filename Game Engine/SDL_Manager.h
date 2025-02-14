#pragma once

#include <SDL.h>

#define MAXWINDOWS 8

class SDL_Manager {
private:
	SDL_Surface* buffers[MAXWINDOWS] = {};
	SDL_Window* windows[MAXWINDOWS] = {};
	SDL_GLContext context = NULL;
	size_t count;

	~SDL_Manager();
	SDL_Manager();

public:
	static SDL_Manager& sdl();
	void closeWindow(uint32_t id);
	void spawnWindow(const char* title, int width, int height, SDL_bool resizable);
	void handleResize(uint32_t id);
	void updateWindows();

	// force exclusion of copy constructor and copy assignment
	SDL_Manager(const SDL_Manager&) = delete;
	SDL_Manager& operator=(const SDL_Manager&) = delete;
};

