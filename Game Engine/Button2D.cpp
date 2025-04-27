#include "Button2D.h"

Button2D::Button2D(glm::vec2 minIn, glm::vec2 maxIn, bool showIn) {
	min = minIn;
	max = maxIn;
	show = showIn;
	action = nullptr;
}
void Button2D::assignAction(void (*actionIn)(void*)) {
	action = actionIn;
}

bool Button2D::has(glm::vec2 p) { // whether given point is in button area
	return (min.x < p.x && max.x > p.x && min.y < p.y && max.y > p.y);
}