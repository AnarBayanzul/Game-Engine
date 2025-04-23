#pragma once

#include <SDL.h>

enum PLAYTYPE {
	ONCE = 0,
	LOOP
};

struct SoundStates {
	Uint8* startPoint;
	Uint32 length;
	float volume;
	PLAYTYPE type;
	Uint8* initialStart;
	Uint32 initialLength;
};