#include "Engine.h"
#include "quat.h"
#include "Utility.h" // TODO this might be unnecessary
#include "Render.h"
#include "RenderCel.h"
#include "Texture.h"


#include "SoundSystem.h"


#include <iostream>
#include <thread> // TODO unneeded
// This is source file for end user's application layer

#include <glm/glm.hpp>
#include <glm/ext.hpp>


static void example() {
	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", glm::perspective(1.309f, 1.0f, 0.1f, 100.0f));
	int monkeyMesh = renderObject->addMesh("monkeySmoothNormal.txt", false);
	int cubeMesh = renderObject->addMesh("uvCube.txt", false);
	if (monkeyMesh == -1) {
		throw;
	}
	Texture* example = new Texture("ohMyGah.bmp", 0);
	int texIndex = renderObject->addTexture(example);


	GameObject* monkey = new GameObject(
		glm::vec3(-1.0f, 0.0f, -2.5f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
		monkeyMesh,
		texIndex,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.2f, 0.0f),
		glm::vec4(0.8f, 0.7f, 0.1f, 1.0f),
		true
	);
	GameObject* cube = new GameObject(
		glm::vec3(1.0f, 0.0f, -4.0f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), 3.1415f / 9.0f),
		cubeMesh,
		texIndex,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -0.2f, 0.0f),
		glm::vec4(0.1f, 0.1f, 1.0f, 1.0f),
		true
	);
	int monkeyIndex = renderObject->addObject(monkey);
	if (monkeyIndex == -1) {
		throw;
	}
	int cubeIndex = renderObject->addObject(cube);
	if (cubeIndex == -1) {
		throw;
	}
	addToRenderQueue(renderObject);
}

static void exampleCel() {
	RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", glm::perspective(1.309f, 1.0f, 0.1f, 100.0f));
		
	
	int monkeyMesh = renderObject->addMesh("monkeySmoothNormal.txt", false);
	int cubeMesh = renderObject->addMesh("mesh.txt", false);
	if (monkeyMesh == -1) {
		throw;
	}
	GameObject* monkey = new GameObject(
		glm::vec3(1.0f, 0.0f, -2.5f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
		monkeyMesh,
		-1,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.2f, 0.0f),
		glm::vec4(0.8f, 0.7f, 0.1f, 1.0f),
		true
	);
	GameObject* cube = new GameObject(
		glm::vec3(-1.0f, 0.0f, -4.0f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f),
		cubeMesh,
		-1,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		true
	);
	int monkeyIndex = renderObject->addObject(monkey);
	if (monkeyIndex == -1) {
		throw;
	}
	int cubeIndex = renderObject->addObject(cube);
	if (cubeIndex == -1) {
		throw;
	}
	addToRenderQueue(renderObject);
}

int initialize() { // What should default initialize look like? 
	std::cout << "user successfully performs initialization\n";

	example();
	//exampleCel();
	SoundSystem::system().loadSound("ohMyGah.wav");
	SoundSystem::system().playSound(0);

	return 0;
}

int globalUpdate(float deltaSec) {
	// TODO is this necessary?
	return 0;
}