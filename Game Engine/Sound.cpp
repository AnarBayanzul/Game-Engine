#include "Sound.h"

Sound::Sound(std::string fileName) {
	if (!SDL_LoadWAV(fileName.c_str(), &spec, audio_buf, audio_len)) {
		throw;
	}
}

Sound::~Sound() {
	SDL_FreeWAV(*audio_buf);
}