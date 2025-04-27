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
#include "FrameBuffer.h"
#include "Button2D.h"
#include "PointLight.h"
#include "ScreenAnimation.h"
#include "PointLightBuffer.h"
#include "BloomBuffer.h"

#include <GL/glew.h>
#include <string>
#include <set>
#include <utility>

#define MAXOBJECTS 256
#define MAXMESHES 128
#define MAXTEXTURES 128
#define MAXCAMERAS 16
#define MAXANIMATIONS 16
#define MAXSANIMATIONS 2
#define MAXPLAYINGANIMATIONS 64
#define ANIMATIONBONES 19 // TODO this shouldn't exist
#define MAXPOINTLIGHTS 32
#define MAXBUTTONS 128

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
	int pointLightCount;
	int buttonCount;
	int sAnimationCount;
	GLuint program;
	Texture* textures[MAXTEXTURES];
	Mesh* meshes[MAXMESHES] = {};
	Mesh* physicsMeshes[MAXMESHES] = {}; // Convex versions of meshes (indices are one to one with meshes)
	Animation* animations[MAXANIMATIONS] = {};
	GameObject* objects[MAXOBJECTS] = {};
	Camera* cameras[MAXCAMERAS] = {};
	ScreenAnimation sAnimations[MAXSANIMATIONS] = {};

	PointLightBuffer* fBuffer;
	BloomBuffer* bBuffer;
	glm::vec3 lightPositions[MAXPOINTLIGHTS] = {};
	glm::vec3 lightColors[MAXPOINTLIGHTS] = {};
	PointLight* pointLights[MAXPOINTLIGHTS] = {};
	

	AnimationStates playback[MAXPLAYINGANIMATIONS] = {};
	int playbackSize;

	GLuint loadShaders(std::string vertFile, std::string fragFile);
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
	Button2D* buttons[MAXBUTTONS] = {};
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
	virtual void generateFrameBuffer();
	virtual void update(float delta);
	Mesh** getMeshes();

	int addCamera(Camera* cam);
	void setCamera(int camIndex);

	int addPointLight(PointLight* light);
	int addPointLight(glm::vec3 position, glm::vec4 color);
	void removePointLight(int index);

	int addButton(Button2D* button);
	void removeButton(int index);

	float updateSAnimation(float delta);
	int addSAnimation(ScreenAnimation sAnimation);




	Render(std::string vertFile, std::string fragFile, Camera* CameraIn);
	~Render();
};

void draw(RenderInfo info, GameObject* object);

int addToRenderQueue(Render* renderObject);