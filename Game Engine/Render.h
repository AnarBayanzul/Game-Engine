#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Animation.h"
#include "Camera.h"
#include "Node.h"
#include "tile.h"
#include "Engine.h"
#include "AnimationStates.h"

#include <GL/glew.h>
#include <string>
#include <set>
#include <utility>

#define MAXOBJECTS 256
#define MAXMESHES 128
#define MAXTEXTURES 128
#define MAXCAMERAS 16
#define MAXANIMATIONS 16
#define MAXPLAYINGANIMATIONS 64
#define ANIMATIONBONES 19 // TODO this shouldn't exist

#define GRIDSIZE 32 // this is gridsize on a side
#define WORLDMIN -128.0
#define WORLDMAX 128.0

class Render { // One program per render object
protected:
	int objectCount;
	int meshCount;
	int textureCount;
	int animationCount;
	int cameraCount;
	int activeCamera;
	GLuint program;
	Texture* textures[MAXTEXTURES];
	Mesh* meshes[MAXMESHES] = {};
	Mesh* physicsMeshes[MAXMESHES] = {}; // Convex versions of meshes (indices are one to one with meshes)
	Animation* animations[MAXANIMATIONS] = {};
	GameObject* objects[MAXOBJECTS] = {};
	Camera* cameras[MAXCAMERAS] = {};

	AnimationStates playback[MAXPLAYINGANIMATIONS] = {};
	int playbackSize;

	int loadShaders(std::string vertFile, std::string fragFile);
	GLint uniformIndexProj;
	GLint uniformIndexTran;
	GLint uniformIndexColor;
	GLint uniformIndexBones;

	// TODO for AABB
	void assignAABB(GameObject* obj);

	std::set<std::pair<GameObject*, GameObject*>> collisionSet;
	const float TILESIZE = (WORLDMAX - WORLDMIN) / (float)GRIDSIZE;
	tile collisionGrid[GRIDSIZE][GRIDSIZE][GRIDSIZE];
	void initializeGrid(); // Call only once
	void addToGrid(GameObject* obj);
	// make sure this func has old min and max values
	void removeFromGrid(GameObject* obj);
	void generateCollisionSet();


public:
	Node* root;
	// these methods return index
	// these two are for convex meshes
	int addMesh(std::string fileName, bool bytes, bool boned);
	int addMesh(Mesh* inputMesh); // conceivably only needed when wanting to add already loaded mesh to different render
	// these two are to define your own physics meshes
	int addMesh(Mesh* inputMesh, Mesh* inputPhysicsMesh);
	int addMesh(std::string fileName, std::string physicsFileName, bool bytes, bool boned);

	int addAnimation(std::string fileName, bool bytes);
	int playAnimation(int animationIndex, int objIndex);
	bool stopAnimation(int playBackIndex);
	void updateAnimation(float delta);

	int addTexture(std::string fileName);
	int addTexture(Texture* inputTexture);
	int addObject(GameObject* obj);
	GameObject** getObjects();
	Camera* getActiveCamera();
	Camera* getCamera(int camIndex);
	virtual void update(float delta);
	Mesh** getMeshes();

	int addCamera(Camera* cam);
	void setCamera(int camIndex);







	Render(std::string vertFile, std::string fragFile, Camera* CameraIn);
	~Render();
};

void draw(RenderInfo info, GameObject* object);

int addToRenderQueue(Render* renderObject);