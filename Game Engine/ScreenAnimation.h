#pragma once

enum transitionType {
	FADEOUT,
	FADEIN,
	DARK
};

struct ScreenAnimation {
	float time;
	float duration;

	transitionType type;
	void* other;
	void* data;
	void (*callback)(void*, void*);
};