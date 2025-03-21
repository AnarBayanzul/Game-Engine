#pragma once
//#include "Render.h"
#include "Camera.h"
#include "renderInfo.h"

#include <vector>
#include <glm/glm.hpp>


class Node {
private:
	std::vector<Node*> children;
	GameObject* object; // the Game object or whatever
	glm::vec4 boundingBox[2]; // TODO or some other way to organize graph structure

public:
	bool show; // if false, then will also hide children
	Node();
	Node(GameObject* object);
	~Node();
	int addChild(Node* node);
	int popChild();
	void render(RenderInfo info, Camera* camera, glm::mat4 parentTran, void (*drawCall)(RenderInfo, GameObject*, glm::mat4)); // TODO arguments may vary depending on how render queue implemented
	bool shouldRender(Camera* camera, glm::mat4 parentTran);
	GameObject* getObject();
};

