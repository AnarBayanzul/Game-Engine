#pragma once

// TODO: header guard
#include <SDL.h>
#include <iostream> // for strings

#define MAXWINDOWS 8

class SDL_Manager {
private:
	SDL_Surface* buffers[MAXWINDOWS] = {};
	SDL_Window* windows[MAXWINDOWS] = {};
	size_t count;

	~SDL_Manager();
	SDL_Manager();

public:
	static SDL_Manager& sdl(); // TODO: i don't understand ampersand at end
	void closeWindow(uint32_t id);
	void spawnWindow(const char* title, int width, int height, SDL_bool resizable);
	void handleResize(uint32_t id);
	void updateWindows();

	// force exclusion of copy constructor and copy assignment
	SDL_Manager(const SDL_Manager&) = delete;
	SDL_Manager& operator=(const SDL_Manager&) = delete;
};

