#pragma once
#include "GameObject.h"
#include "Animation.h"

struct AnimationStates {
	float time;
	GameObject* obj;
	Mesh* meshIn;
	Animation* anim;
};