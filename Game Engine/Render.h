#pragma once
#include "GameObject.h"
#include "Mesh.h"

#include <GL/glew.h>
#include <string>

#define MAXOBJECTS 256
#define MAXMESHES 128
#define MAXTEXTURES 128

class Render { // One program per render object
public:
	int objectCount;
	int meshCount;
	int textureCount;
	GLuint program;
	GLuint textures[MAXTEXTURES] = {};
	Mesh* meshes[MAXMESHES] = {};
	GameObject* objects[MAXOBJECTS] = {};
	glm::mat4 perspective; // TODO change, not a good name, should be projection

	int loadShaders(std::string vertFile, std::string fragFile);
	GLint uniformIndexProj;
	GLint uniformIndexTran;
	GLint uniformIndexColor;
public:
	// these methods return index
	int addMesh(std::string fileName, bool bytes);
	int addMesh(Mesh* inputMesh); // conceivably only needed when wanting to add already loaded mesh to different render
	int addTexture(std::string fileName);
	int addObject(GameObject* obj);
	virtual void update(float delta);

	Render(std::string vertFile, std::string fragFile, glm::mat4 proj);
	~Render();
};

