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


#define MAXRENDEROBJECTS 2
extern Render* RenderQueue[MAXRENDEROBJECTS];
extern int renderCount;



SDL_Surface* parseTexture(std::string fileName);

size_t parseMesh(std::string fileName, std::vector<float>& data, std::vector<vertexBone>& vertexBoneData, std::vector<boneData>& bones, bool bytes, bool boned);

size_t parseAnim(std::string fileName, int& boneCount, std::vector<Keyframe>& data, bool bytes);