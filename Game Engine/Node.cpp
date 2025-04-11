#include "Node.h"

Node::Node() {
	children = {};
	object = nullptr;
	show = true;
}

Node::Node(GameObject* objectIn) {
	children = {};
	object = objectIn;
	show = true;
}

Node::~Node() {
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}
}

// returns index of child
int Node::addChild(Node* node) {
	children.push_back(node);
	return children.size() - 1;
}

// returns index of popped child
int Node::popChild() {
	children.pop_back();
	return children.size();
}

bool Node::shouldRender(Camera* camera, glm::mat4 parentTran) {
	if (!show) {
		return false;
	}
	// decide if intersecting or something to choose whether to render

	camera->calculatePlanes();
	//camera->planes; // TODO might need to offset by parentTran
	// TODO

	return true;
}

void Node::render(RenderInfo info, Camera* camera, glm::mat4 parentTran, void (*drawCall)(RenderInfo, GameObject*)) {
	glm::mat4 currentTran;
	if (object == nullptr) {
		currentTran = parentTran;
	} else {
		currentTran = parentTran * object->getModel();
	}
	for (int i = 0; i < children.size(); ++i) {
		if (shouldRender(camera, parentTran)) {
			children[i]->object->setParentTransform(currentTran);
			drawCall(info, children[i]->object);
			children[i]->render(info, camera, children[i]->object->getModel() * parentTran, drawCall);
		}
	}
}

GameObject* Node::getObject() {
	return object;
}
