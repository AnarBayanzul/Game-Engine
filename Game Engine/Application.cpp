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
	//Texture* example = new Texture("ohTheMisery.bmp", 0);
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
	int cubeMesh = renderObject->addMesh("uvCube.txt", false);
	if (monkeyMesh == -1) {
		throw;
	}
	Texture* example = new Texture("ohMyGah.bmp", 0);
	int texIndex = renderObject->addTexture(example);

	GameObject* monkey = new GameObject(
		glm::vec3(1.0f, 0.0f, -2.5f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
		monkeyMesh,
		texIndex,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.2f, 0.0f),
		glm::vec4(0.8f, 0.7f, 0.1f, 1.0f),
		true
	);
	GameObject* cube = new GameObject(
		glm::vec3(-1.0f, 0.0f, -4.0f),
		quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f),
		cubeMesh,
		texIndex,
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

static void exampleAudio() {
	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", glm::perspective(1.309f, 1.0f, 0.1f, 100.0f));
	
	int cubeMesh = renderObject->addMesh("uvCube.txt", false);

	Texture* example = new Texture("ohMyGah.bmp", 0);
	int texIndex = renderObject->addTexture(example);

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
	int cubeIndex = renderObject->addObject(cube);
	if (cubeIndex == -1) {
		throw;
	}

	SoundSystem::system().loadSound("ohMyGah.wav");
	SoundSystem::system().playSound(0);
	SoundSystem::system().playSound("ohTheMisery.wav");


	addToRenderQueue(renderObject);
}





int snakeLength = 0;
int oldDirection[2] = { 0, 1 };
int newDirection[2] = { 0, 1 };
int headLocation[2] = { 5, 5 };
int fruitLocation[2];
int headSize = 2;
#define MAPSIZE 10
int map[MAPSIZE][MAPSIZE] = {};
Render* renderObject;

int snakeTexIndex;
int headTexIndex;
int tailTexIndex;

bool lost = false;

static void snakeInit() {
	//RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", glm::perspective(1.309f, 1.0f, 0.1f, 100.0f));
	renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", glm::perspective(1.309f, 1.0f, 0.1f, 100.0f));

	int snakeBodyMesh = renderObject->addMesh("uvCube.txt", false);

	Texture* snakeBodyTex = new Texture("ohTheMisery.bmp", 0);
	snakeTexIndex = renderObject->addTexture(snakeBodyTex);
	Texture* tailTex = new Texture("ohTheMisery.bmp", 0);
	tailTexIndex = renderObject->addTexture(tailTex);
	Texture* headTex = new Texture("ohMyGah.bmp", 0);
	headTexIndex = renderObject->addTexture(headTex);
	
	int fruitMesh = renderObject->addMesh("monkeySmoothNormal.txt", false);
	Texture* fruitTex = new Texture("ohMyGah.bmp", 0);
	int fruitTexIndex = renderObject->addTexture(snakeBodyTex);


	// 0, 0 is bottom left
	map[5][5] = 2;
	map[5][4] = 1;
	for (int i = 0; i < MAPSIZE; ++i) {
		for (int j = 0; j < MAPSIZE; ++j) {
			renderObject->addObject(
				new GameObject(
					glm::vec3(2.0f * i - 10.0f + 1, 2.0f * j - 10.0f + 1, -15.0f),
					quat(),
					snakeBodyMesh,
					snakeTexIndex,
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
					(bool)map[i][j]
				)
			);
		}
	}
	do { // find empty space for fruit
		fruitLocation[0] = rand() % MAPSIZE;
		fruitLocation[1] = rand() % MAPSIZE;
		std::cout << "initial fruit: " << fruitLocation[0] << " " << fruitLocation[1] << std::endl;
	} while (map[fruitLocation[0]][fruitLocation[1]]);
	map[fruitLocation[0]][fruitLocation[1]] = -1;

	// add fruit object
	renderObject->addObject(
		new GameObject(
			glm::vec3(2.0f * fruitLocation[0] - 10.0f + 1, 2.0f * fruitLocation[1] - 10.0f + 1, -15.0f),
			quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
			fruitMesh,
			fruitTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	// set head tex
	renderObject->objects[5 * MAPSIZE + 5]->setTextureElement(headTexIndex);


	addToRenderQueue(renderObject);

	SoundSystem::system().loadSound("ohTheMisery.wav");
	SoundSystem::system().playSound("ohMyGah.wav");
}
float wait = 0.1;
float elapsed = 0.0f;
void snakeUpdate(float deltaSec) {
	if (lost) {
		return;
	}
	elapsed += deltaSec;
	if (elapsed > wait) {
		elapsed = 0.0f;
		// actual update
		system("cls");
		// take direction input
		switch (lastKey.keysym.sym) {
		case SDLK_LEFT:
			std::cout << "left\n";
			newDirection[0] = -1;
			newDirection[1] = 0;
			break;
		case SDLK_RIGHT:
			std::cout << "right\n";
			newDirection[0] = 1;
			newDirection[1] = 0;
			break;
		case SDLK_UP:
			std::cout << "up\n";
			newDirection[0] = 0;
			newDirection[1] = 1;
			break;
		case SDLK_DOWN:
			std::cout << "down\n";
			newDirection[0] = 0;
			newDirection[1] = -1;
			break;
		default:
			break;
		}

		// check if direction valid
		if (abs(oldDirection[0] - newDirection[0]) == 2 || abs(oldDirection[1] - newDirection[1]) == 2) {
			// invalid direction
			newDirection[0] = oldDirection[0];
			newDirection[1] = oldDirection[1];
		}
		else {
			oldDirection[0] = newDirection[0];
			oldDirection[1] = newDirection[1];
		}
		// update head location
		headLocation[0] += oldDirection[0];
		headLocation[1] += newDirection[1];
		// map wraparound
		if (headLocation[0] >= MAPSIZE) {
			headLocation[0] -= MAPSIZE;
		} else if (headLocation[0] < 0) {
			headLocation[0] += MAPSIZE;
		}
		if (headLocation[1] >= MAPSIZE) {
			headLocation[1] -= MAPSIZE;
		} else if (headLocation[1] < 0) {
			headLocation[1] += MAPSIZE;
		}
		// check if new head location has fruit
		if (map[headLocation[0]][headLocation[1]] == -1) {
			// has fruit
			++headSize;
			SoundSystem::system().playSound(0);
			do { // find empty space for fruit
				fruitLocation[0] = rand() % MAPSIZE;
				fruitLocation[1] = rand() % MAPSIZE;
			} while (map[fruitLocation[0]][fruitLocation[1]]);
			map[fruitLocation[0]][fruitLocation[1]] = -1;
			renderObject->objects[MAPSIZE * MAPSIZE]->move(glm::vec3(2.0f * fruitLocation[0] - 10.0f + 1, 2.0f * fruitLocation[1] - 10.0f + 1, -15.0f));
		}
		else if (map[headLocation[0]][headLocation[1]] > 0) {
			// has snake already
			lost = true;
			std::cout << "YOU LOSE\nSCORE: " << std::dec << headSize;
			return;
		} else {
			// no fruit
			// move snake
			for (int i = 0; i < MAPSIZE; ++i) {
				for (int j = 0; j < MAPSIZE; ++j) {
					if (map[i][j] > 0) {
						--map[i][j];
					}
				}
			}

		}
		//std::cout << "\tHead Loc: " << headLocation[0] << " " << headLocation[1] << std::endl;
		map[headLocation[0]][headLocation[1]] = headSize;


		// update snakeMap view
		for (int i = 0; i < MAPSIZE; ++i) {
			for (int j = 0; j < MAPSIZE; ++j) {
				if (map[i][j] == -1) {
					// TODO show fruit
					continue;
				} else if (map[i][j] == 1) { // check if head tail or body
					renderObject->objects[i * MAPSIZE + j]->setTextureElement(tailTexIndex);
				} else if (map[i][j] == headSize) {
					renderObject->objects[i * MAPSIZE + j]->setTextureElement(headTexIndex);
				} else {
					renderObject->objects[i * MAPSIZE + j]->setTextureElement(snakeTexIndex);
				}
				renderObject->objects[i * MAPSIZE + j]->show = (bool)map[i][j];
			}
		}
	}
}



int initialize() { // What should default initialize look like? 
	std::cout << "user successfully performs initialization\n";

	//example();
	//exampleCel();
	//exampleAudio();

	snakeInit();


	return 0;
}

int globalUpdate(float deltaSec) {
	// TODO is this necessary?

	snakeUpdate(deltaSec);

	return 0;
}