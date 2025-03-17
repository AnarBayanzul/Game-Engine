#include "SoundSystem.h"
#include <iostream>
#include <string>

void callback(void* userData, Uint8* stream, int length) { // length of stream
	// Initialize mix values to 0
		// then iterate over playback SoundStates to mix in additional audio chunks
	Uint8* mix = SoundSystem::system().getMix();
	std::vector<Sound> sounds = SoundSystem::system().getSounds();
	std::vector<SoundStates>& playback = SoundSystem::system().getPlayback();

	for (int i = 0; i < length; ++i) {
		mix[i] = 0;
	}

	int toPlay;
	for (int i = 0; i < playback.size(); ++i) {
		toPlay = (playback[i].length > length ? length : playback[i].length);
		for (int j = 0; j < toPlay; ++j) {
			mix[j] += playback[i].startPoint[j];
		}
		// move startPoint forward
		playback[i].startPoint += toPlay;
		// decrement length
		playback[i].length -= toPlay;
	}
	for (int i = playback.size() - 1; i >= 0; --i) {
		// pop if playback length is zero
		if (playback[i].length <= 0) {
			playback.pop_back();
		}
	}
	// TODO i think this is what you're supposed to do:
	memcpy(stream, mix, length);

}

SoundSystem& SoundSystem::system() {
	static SoundSystem instance;
	return instance;
}

SoundSystem::SoundSystem() {
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		throw;
	}
	//std::cout << SDL_GetNumAudioDevices(0);
	SDL_AudioSpec desired = {};
	//desired.freq = 22050;
	//desired.format = AUDIO_S16;
	//desired.channels = 2;
	//desired.samples = 1024;
	//desired.userdata = NULL;
	desired.callback = callback;
	SDL_AudioSpec obtained = {};
	device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &desired, &obtained, 0);

	// allocate mix size
	mixSize = obtained.size;
	mix = new Uint8[mixSize];
	SDL_PauseAudioDevice(device, 0);
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
	//sounds.back().spec.callback = callback;
	playback.push_back(SoundStates{sounds.back().audio_buf, sounds.back().audio_len});

	return true;
}

bool SoundSystem::playSound(int index) {
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

Uint8* SoundSystem::getMix() {
	return mix;
}

std::vector<SoundStates>& SoundSystem::getPlayback() {
	return playback;
}

std::vector<Sound>& SoundSystem::getSounds() {
	return sounds;
}