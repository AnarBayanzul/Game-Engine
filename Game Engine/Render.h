#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Node.h"

#include <GL/glew.h>
#include <string>

#define MAXOBJECTS 256
#define MAXMESHES 128
#define MAXTEXTURES 128

class Render { // One program per render object
protected:
	int objectCount;
	int meshCount;
	int textureCount;
	GLuint program;
	Texture* textures[MAXTEXTURES];
	Mesh* meshes[MAXMESHES] = {};
	GameObject* objects[MAXOBJECTS] = {};
	Camera* camera; // TODO for now, support for only one;

	int loadShaders(std::string vertFile, std::string fragFile);
	GLint uniformIndexProj;
	GLint uniformIndexTran;
	GLint uniformIndexColor;
public:
	Node* root;
	// these methods return index
	int addMesh(std::string fileName, bool bytes);
	int addMesh(Mesh* inputMesh); // conceivably only needed when wanting to add already loaded mesh to different render
	int addTexture(std::string fileName);
	int addTexture(Texture* inputTexture);
	int addObject(GameObject* obj);
	GameObject** getObjects();
	Camera* getCamera();
	virtual void update(float delta);


	Render(std::string vertFile, std::string fragFile, Camera* CameraIn);
	~Render();
};

