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
	void callback(void*, Uint8*, int);
public:
	SoundSystem();
	~SoundSystem();
	bool loadSound(std::string fileName);
	bool playSound(int);
	bool playSound(std::string fileName);
};