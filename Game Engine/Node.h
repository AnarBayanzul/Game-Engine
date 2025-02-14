#pragma once

class Node {
private:
	Node* children;
	void* objReference;
public:
	Node();
	void Render(); // TODO arguments may vary depending on how render queue implemented
};

