#include "Sound.h"
#include <iostream>
//#include <iostream>

Sound::Sound(std::string fileName) {
	if (!SDL_LoadWAV(fileName.c_str(), &spec, &audio_buf, &audio_len)) {
		throw;
	}
	spec.userdata = NULL;
	//std::cout << fileName << std::endl;
	//std::cout << "\tLength: " << std::dec << audio_len << std::endl;
	//std::cout << "\tChannels: " << (int)spec.channels << std::endl;
	//std::cout << "\tFrequency: " << std::dec << spec.freq << std::endl;
	//std::cout << "\tFormat: " << std::hex << spec.format << std::endl; // ohMyGah.wav is SDL_AUDIO_S16LE (signed 16-bit samples) SDL_DEFINE_AUDIO_FORMAT(1, 0, 0, 16)
	//std::cout << "\tSamples:" << std::dec << spec.samples << std::endl;
	//std::cout << "\tSilence:" << std::hex << spec.silence << std::endl;
}

Sound::~Sound() {
	std::cout << "oh the misery";
	SDL_FreeWAV(audio_buf);
}