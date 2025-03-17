#pragma once
#include "Render.h"

#include <string>
// For external global functions, the API
// Intention is for source file to remain hidden
// Including additional libraries for the user is useful here

#define MAXRENDEROBJECTS 2
extern Render* RenderQueue[MAXRENDEROBJECTS];
extern int renderCount;

// Declared in Application.cpp
int initialize();
int globalUpdate(float deltaSec);

// Rest declared in Engine.cpp
int addToRenderQueue(Render* renderObject);

size_t getDeltaTime();
float getDeltaSeconds();