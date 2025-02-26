#pragma once
// For internal global variables and functions (hidden from end user space)
#include "Mesh.h"
#include "GameObject.h"
#include "Render.h"

#include <GL/glew.h>

#include <string>
#include <chrono>
#include <SDL_surface.h>

// For timing
extern size_t delta;
extern float deltaSec;


SDL_Surface* parseTexture(std::string fileName);

size_t parseMesh(std::string filename, std::vector<float>& data, bool bytes);