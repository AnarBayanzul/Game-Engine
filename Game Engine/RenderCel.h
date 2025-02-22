#pragma once

#include "Render.h"

class RenderCel : public Render {
private:
	//float continuity = 10.0f;
public:
	using Render::Render;
	void update(float delta);
};

