#pragma once

#include <string>
#include <SDL.h>

class Sound {
private:
	Uint8** audio_buf;
	Uint32* audio_len;
	SDL_AudioSpec spec;
public:
	Sound(std::string fileName);
	~Sound();
};

