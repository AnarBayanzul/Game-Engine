#include "Engine.h"
#include "Utility.h"

#include <string>
#include <fstream>

SDL_KeyboardEvent lastKey;

Render* RenderQueue[MAXRENDEROBJECTS];
int renderCount = 0;

int addToRenderQueue(Render* renderObject) {
	RenderQueue[renderCount] = renderObject;
	return renderCount++ ;
}

size_t getDeltaTime() {
	return delta;
}

float getDeltaSeconds() {
	return deltaSec;
}