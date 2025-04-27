#pragma once

#include <glm/glm.hpp>

// axis aligned box, light weight
class Button2D {
private:
	bool show;
	glm::vec2 min;
	glm::vec2 max;
	void (*action)(void*);
public:
	Button2D(glm::vec2 minIn, glm::vec2 maxIn, bool showIn);
	void assignAction(void (*actionIn)(void*));
	bool has(glm::vec2 p); // whether given point is in button area
};

