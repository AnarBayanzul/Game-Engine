#pragma once

#include "Sound.h"
#include "SoundStates.h"

#include <vector>
#include <string>
#include <SDL_audio.h>

class SoundSystem {
private:
	std::vector<Sound> sounds; // Ideally for small sound objects
	SDL_AudioDeviceID device;
	std::vector<SoundStates> playback;
	Uint8* mix;
	int mixSize;
	SoundSystem();
	~SoundSystem();
public:
	Uint8* getMix();
	std::vector<SoundStates>& getPlayback();
	std::vector<Sound>& getSounds();
	bool loadSound(std::string fileName);
	bool playSound(int index);
	bool playSound(std::string fileName);

	// enforce singleton
	static SoundSystem& system();
	// force exclusion of copy constructor and copy assignment
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
};

void callback(void* userData, Uint8* stream, int length);