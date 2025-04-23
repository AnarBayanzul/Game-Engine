#pragma once

#include "Sound.h"
#include "SoundStates.h"

#include <vector>
#include <string>
#include <SDL_audio.h>

#define MAXTRACKS 16

class SoundSystem {
private:
	std::vector<Sound*> sounds; // Ideally for small sound objects
	SDL_AudioDeviceID device;
	SoundStates playback[MAXTRACKS] = {};
	int playbackSize;
	Uint8* mix;
	int mixSize;
	SoundSystem();
	~SoundSystem();
public:
	Uint8* getMix();
	int getMixSize();
	SoundStates* getPlayback();
	int getPlaybackSize();
	void setPlaybackSize(int size);
	std::vector<Sound*>& getSounds();
	bool loadSound(std::string fileName);
	bool playSound(int index);
	bool playSound(std::string fileName);
	// volume should be between 0.0 and 1.0
	bool playSound(int index, float volume, PLAYTYPE type);
	bool playSound(std::string fileName, float volume, PLAYTYPE type);

	Sound* runtimeSound;

	// enforce singleton
	static SoundSystem& system();
	// force exclusion of copy constructor and copy assignment
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
};

void callback(void* userData, Uint8* stream, int length);