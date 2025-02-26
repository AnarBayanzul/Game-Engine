#include "SoundSystem.h"
#include <iostream>
#include <string>

void SoundSystem::callback(void*, Uint8*, int) {
	// Initialize mix values to 0
		// then iterate over playback SoundStates to mix in additional audio chunks
	// needs to be a free function (by some means)
		// will be passed to SDL_AudioSpec callback pointer
}

SoundSystem::SoundSystem() {
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		throw;
	}
	std::cout << SDL_GetNumAudioDevices(0);
	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;
	device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &desired, &obtained, 0);

	// allocate mix size
	mix = new Uint8[obtained.size];

	SDL_PauseAudioDevice(device, 0);

	// potentially allocate the size of mix array
}

SoundSystem::~SoundSystem() {
	// Pause audio device
	// cleanup loaded audio
	// cleanup mix
	 
	
	//SDL_FreeWAV();
	delete[] mix;
	SDL_CloseAudioDevice(device);
}

bool SoundSystem::loadSound(std::string fileName) {
	// load filepath wav file
	// create a Sound object that is pushed on the sounds vector
	// return true if successful
	sounds.push_back(Sound(fileName));

	return true;
}

bool SoundSystem::playSound(int) {
	// play sound indexing sounds vector


	return true;
}
bool SoundSystem::playSound(std::string fileName) {
	// load sound not from sounds container and play
	// expected to only support one runtime loaded sound at a time
	// useful for large track:
		// dialogue
		// music


	Sound s = Sound(fileName);

	return true;
}