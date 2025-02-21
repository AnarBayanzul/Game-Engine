#pragma once
// For internal global variables and functions (hidden from end user space)
#include "Mesh.h"
#include "GameObject.h"
#include "Render.h"

#include <GL/glew.h>

#include <string>
#include <chrono>

// For timing
extern size_t delta;
extern float deltaSec;



size_t parseMesh(std::string filename, std::vector<float>& data, bool bytes);