#pragma once
// For internal global variables and functions (hidden from end user space)
#include "Mesh.h"
#include "GameObject.h"

#include <GL/glew.h>

#include <string>
#include <chrono>

// For the shader program
extern GLuint program;

extern Mesh* cubeMesh;
extern GameObject* cube;
extern GLint uniformIndexProj;
extern GLint uniformIndexTran;
extern GLint uniformIndexColor;

// For timing
extern std::chrono::milliseconds delta;
extern std::chrono::seconds deltaSec;


int loadShaders(std::string vertFile, std::string fragFile);