#include "Sound.h"

#include <iostream>

Sound::Sound(std::string fileName) {
	if (!SDL_LoadWAV(fileName.c_str(), &spec, &audio_buf, &audio_len)) {
		throw;
	}
	spec.userdata = NULL;
	std::cout << "audio:\n";
	std::cout << audio_len << std::endl;
	std::cout << spec.channels << std::endl;
	std::cout << spec.freq << std::endl;
	std::cout << std::hex << spec.format << std::endl; // ohMyGah.wav is SDL_AUDIO_S16LE (signed 16-bit samples) SDL_DEFIEN_AUDIO_FORMAT(1, 0, 0, 16)
}

Sound::~Sound() {
	SDL_FreeWAV(audio_buf);
}