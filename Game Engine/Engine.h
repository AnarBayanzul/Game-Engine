#pragma once
#include "quat.h"
#include "Mesh.h"

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>
// For external global functions, the API
// Intention is for source file to remain hidden
// Including additional libraries for the user is useful here


extern SDL_KeyboardEvent lastKey;


// Declared in Application.cpp
int initialize();
int globalUpdate(float deltaSec);



size_t getDeltaTime();
float getDeltaSeconds();

// Integration functions
glm::vec3 integrateLinear(float deltaTime, const glm::vec3& linear);
quat integrateAngular(float deltaTime, const glm::vec3 angular);

// Collisions stuff
bool GJK(Mesh* A, quat aQ, glm::vec3 sA, Mesh* B, quat bQ, glm::vec3 sB);

//void test1();
//void test2();
//
//void (*funcptr[])() = { test1, test2 };