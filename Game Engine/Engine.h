#pragma once
#include "quat.h"
#include "Mesh.h"
#include "GameObject.h"

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
int mouseClick(SDL_MouseButtonEvent mEvent);



size_t getDeltaTime();
float getDeltaSeconds();

// Integration functions
glm::vec3 integrateLinear(float deltaTime, const glm::vec3& linear);
quat integrateAngular(float deltaTime, const glm::vec3 angular);




// Animation stuff
// t between 0 and 1;
quat slerp(quat a, quat b, float t);







// Collisions stuff
bool GJK(Mesh* A, quat aQ, glm::vec3 sA, Mesh* B, quat bQ, glm::vec3 sB, float& dist);

typedef void (*collisionFunc)(GameObject*, GameObject*);
// A must always be less than B
//   __0_1_2_3  <- A
// 0 | 0
// 1 | 1 2
// 2 | 3 4 5
// 3 | 6 7 8 9
// ^ B
extern collisionFunc collisionTable[(NUMOFTYPES * (NUMOFTYPES + 1)) / 2];


void addToCollisionTable(objectType A, objectType B, collisionFunc inputFunc);
collisionFunc getFromCollisionTable(objectType A, objectType B);