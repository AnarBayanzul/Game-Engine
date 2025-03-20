#include "SoundSystem.h"
#include <iostream>
#include <string>

#include <SDL_endian.h>

#define MIX_MAXVOLUME 128
#define ADJUST_VOLUME(type, s, v) ((s) = (type)(((s) * (v)) / MIX_MAXVOLUME))


void callback(void* userData, Uint8* stream, int length) { // length of stream
	// Initialize mix values to 0
		// then iterate over playback SoundStates to mix in additional audio chunks
	Uint8* mix = SoundSystem::system().getMix();
	std::vector<Sound*> sounds = SoundSystem::system().getSounds();
	SoundStates* playback = SoundSystem::system().getPlayback();
	int playbackSize = SoundSystem::system().getPlaybackSize();


	//std::cout << "Playback info:\n";
	//std::cout << "\tPlayback size: " << playbackSize << std::endl;
	//for (int i = 0; i < playbackSize; ++i) {
	//	std::cout << "\t\t" << i << ": Length: " << playback[i].length << std::endl;
	//	std::cout << "\t\t" << i << ": Head: " << (int)playback[i].startPoint << std::endl;
	//}


	for (int i = 0; i < length; ++i) {
		mix[i] = 0;
	}
	int toPlay;
	int newValue;

	float fvolume = 128.0;
	int volume = (int)SDL_roundf(fvolume * MIX_MAXVOLUME);
	Sint16 src, dst;

	
	for (int i = playbackSize - 1; i >= 0; --i) {
		toPlay = (playback[i].length > length / 2 ? length / 2 : playback[i].length);
		//std::cout << toPlay << std::endl;
		for (int j = 0; j < toPlay; j = j + 2) {
			src = SDL_SwapLE16(*(Sint16*)(playback[i].startPoint + j));
			//ADJUST_VOLUME(Sint16, src, volume);
			dst = SDL_SwapLE16(*(Sint16*)(mix + 2 * j));
			newValue = (int)src + (int)dst;
			if (newValue > SDL_MAX_SINT16) {
				*(Sint16*)(mix + 2 * j) = SDL_MAX_SINT16;
				*(Sint16*)(mix + 2 * j + 2) = SDL_MAX_SINT16;

			} else if (newValue < SDL_MIN_SINT16) {
				*(Sint16*)(mix + 2 * j) = SDL_MIN_SINT16;
				*(Sint16*)(mix + 2 * j + 2) = SDL_MIN_SINT16;
			}
			else {
				*(Sint16*)(mix + 2 * j) = newValue;
				*(Sint16*)(mix + 2 * j + 2) = newValue;
			}
			*(Sint16*)(mix + 2 * j) = SDL_SwapLE16(*(Sint16*)(mix + 2 * j));

			//// Left
			//mix[2 * j] += playback[i].startPoint[j];
			//mix[2 * j + 1] += playback[i].startPoint[j + 1];
			//// Right
			//mix[2 * j + 2] += playback[i].startPoint[j ];
			//mix[2 * j + 3] += playback[i].startPoint[j + 1];
		}
		// move startPoint forward
		playback[i].startPoint += toPlay;
		// decrement length
		playback[i].length -= toPlay;
		if (playback[i].length <= 0) {
			std::swap(playback[i], playback[playbackSize - 1]);
			SoundSystem::system().setPlaybackSize(playbackSize - 1);
		}
	}
	 //TODO i think this is what you're supposed to do:
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
	desired.freq = 44100;
	desired.format = AUDIO_S16;
	//desired.format = AUDIO_S8;
	desired.channels = 2;
	desired.samples = 4096;
	desired.userdata = NULL;
	desired.callback = callback;
	SDL_AudioSpec obtained = {};
	device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(1, 0), 0, &desired, &obtained, 0); // TODO might want different audio device
	if (device == NULL) {
		device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &desired, &obtained, 0); // TODO might want different audio device;
		std::cout << "First device error\n";
	}
	
	std::cout << "System Format:\n";
	std::cout << "\tChannels: " << (int) obtained.channels << std::endl;
	std::cout << "\tFrequency: " << std::dec << obtained.freq << std::endl;
	std::cout << "\tFormat: " << std::hex << obtained.format << std::endl;
	std::cout << "\tSize: " << std::dec << obtained.size << std::endl;
	std::cout << "\tSamples: " << std::dec << obtained.samples << std::endl;
	std::cout << "\tSilence: " << std::hex << obtained.silence << std::endl;


	playbackSize = 0;
	runtimeSound = nullptr;

	// allocate mix size
	mixSize = obtained.size;
	mix = new Uint8[mixSize];
	SDL_PauseAudioDevice(device, 0);
}

SoundSystem::~SoundSystem() {
	// Pause audio device
	// cleanup loaded audio
	// cleanup mix
	for (int i = 0; i < sounds.size(); ++i) {
		delete sounds[i];
	}

	//SDL_FreeWAV();
	delete runtimeSound;
	delete[] mix;
	SDL_CloseAudioDevice(device);
}

bool SoundSystem::loadSound(std::string fileName) {
	// load filepath wav file
	// create a Sound object that is pushed on the sounds vector
	// return true if successful
	sounds.push_back(new Sound(fileName));
	//sounds.emplace_back(fileName);
	//sounds.back().spec.callback = callback;


	//playback.push_back(SoundStates{sounds.back().audio_buf, sounds.back().audio_len});

	return true;
}

bool SoundSystem::playSound(int index) {
	// play sound indexing sounds vector
	playback[playbackSize++] = SoundStates{ sounds[index]->audio_buf, sounds[index]->audio_len };
	return true;
}
bool SoundSystem::playSound(std::string fileName) {
	// load sound not from sounds container and play
	// expected to only support one runtime loaded sound at a time
	// useful for large track:
		// dialogue
		// music
	delete runtimeSound;
	runtimeSound = new Sound(fileName);
	playback[playbackSize++] = SoundStates{ runtimeSound->audio_buf, runtimeSound->audio_len };
	// TODO might want to deal with old playback entry.
	return true;
}

Uint8* SoundSystem::getMix() {
	return mix;
}

SoundStates* SoundSystem::getPlayback() {
	return playback;
}

int SoundSystem::getPlaybackSize() {
	return playbackSize;
}

void SoundSystem::setPlaybackSize(int size) {
	playbackSize = size;
}

std::vector<Sound*>& SoundSystem::getSounds() {
	return sounds;
}

int SoundSystem::getMixSize() {
	return mixSize;
}
