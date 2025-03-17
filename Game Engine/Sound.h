#pragma once

#include <string>
#include <SDL.h>

class Sound {
private:
public:
	Uint8* audio_buf; // contains actual file
	Uint32 audio_len;
	SDL_AudioSpec spec;
	Sound(std::string fileName);
	~Sound();
};

