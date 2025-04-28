#include "Engine.h"
#include "quat.h"
#include "Utility.h" // TODO this might be unnecessary
#include "Render.h"
#include "RenderCel.h"
#include "Texture.h"
#include "Node.h"
#include "Animation.h"


#include "SoundSystem.h"


#include <iostream>
#include <thread> // TODO unneeded
// This is source file for end user's application layer

#include <glm/glm.hpp>
#include <glm/ext.hpp>


static void example() {

	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	int monkeyMesh = renderObject->addMesh("monkeySmoothNormal.txt", false, false);
	int cubeMesh = renderObject->addMesh("uvCube.txt", false, false);
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
	RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	int monkeyMesh = renderObject->addMesh("monkeySmoothNormal.txt", false, false);
	int cubeMesh = renderObject->addMesh("uvCube.txt", false, false);
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
	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	
	int cubeMesh = renderObject->addMesh("uvCube.txt", false, false);

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
	//RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f,WIDTH / HEIGHT, 0.1f, 100.0f));
	renderObject->root = new Node(new GameObject());
	renderObject->root->getObject()->setPosition(glm::vec3(0.0f, 0.0f, -15.0f));
	//renderObject->root->getObject()->setRotation(quat(glm::vec3(0.0f, 1.0f, 0.0f), 3.1415f / 4.0f));

	int snakeBodyMesh = renderObject->addMesh("uvCube.txt", false, false);

	Texture* snakeBodyTex = new Texture("ohTheMisery.bmp", 0);
	snakeTexIndex = renderObject->addTexture(snakeBodyTex);
	Texture* tailTex = new Texture("ohTheMisery.bmp", 0);
	tailTexIndex = renderObject->addTexture(tailTex);
	Texture* headTex = new Texture("ohMyGah.bmp", 0);
	headTexIndex = renderObject->addTexture(headTex);
	
	int fruitMesh = renderObject->addMesh("monkeySmoothNormal.txt", false, false);
	Texture* fruitTex = new Texture("ohMyGah.bmp", 0);
	int fruitTexIndex = renderObject->addTexture(snakeBodyTex);


	// 0, 0 is bottom left
	map[5][5] = 2;
	map[5][4] = 1;
	int objectIndex;
	for (int i = 0; i < MAPSIZE; ++i) {
		for (int j = 0; j < MAPSIZE; ++j) {
			objectIndex = renderObject->addObject(
				new GameObject(
					glm::vec3(2.0f * i - 10.0f + 1, 2.0f * j - 10.0f + 1, 0.0f),
					quat(),
					snakeBodyMesh,
					snakeTexIndex,
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
					(bool)map[i][j]
				)
			);
			renderObject->root->addChild(new Node(renderObject->getObjects()[objectIndex]));
		}
	}
	do { // find empty space for fruit
		fruitLocation[0] = rand() % MAPSIZE;
		fruitLocation[1] = rand() % MAPSIZE;
		//std::cout << "initial fruit: " << fruitLocation[0] << " " << fruitLocation[1] << std::endl;
	} while (map[fruitLocation[0]][fruitLocation[1]]);
	map[fruitLocation[0]][fruitLocation[1]] = -1;

	// add fruit object
	objectIndex = renderObject->addObject(
		new GameObject(
			glm::vec3(2.0f * fruitLocation[0] - 10.0f + 1, 2.0f * fruitLocation[1] - 10.0f + 1, 0.0f),
			quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
			fruitMesh,
			fruitTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	renderObject->root->addChild(new Node(renderObject->getObjects()[objectIndex]));
	// set head tex
	renderObject->getObjects()[5 * MAPSIZE + 5]->setTextureElement(headTexIndex);


	addToRenderQueue(renderObject);

	SoundSystem::system().loadSound("ohTheMisery.wav");
	SoundSystem::system().playSound("ohMyGah.wav");
}
float wait = 0.1;
float elapsed = 0.0f;
static void snakeUpdate(float deltaSec) {
	//renderObject->getObjects()[0]->angularV = glm::vec3(1, 1, 1);
	//std::cout << "obj 0\n";
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << renderObject->getObjects()[0]->getAABBmin()[i];
	//	std::cout << ",";
	//}
	//std::cout << std::endl;
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << renderObject->getObjects()[0]->getAABBmax()[i];
	//	std::cout << ",";
	//}
	//std::cout << std::endl;

	if (lost) {
		return;
	}
	elapsed += deltaSec;
	if (elapsed > wait) {
		elapsed = 0.0f;
		// actual update
		//system("cls");
		// take direction input
		switch (lastKey.keysym.sym) {
		case SDLK_LEFT:
			//std::cout << "left\n";
			newDirection[0] = -1;
			newDirection[1] = 0;
			break;
		case SDLK_RIGHT:
			//std::cout << "right\n";
			newDirection[0] = 1;
			newDirection[1] = 0;
			break;
		case SDLK_UP:
			//std::cout << "up\n";
			newDirection[0] = 0;
			newDirection[1] = 1;
			break;
		case SDLK_DOWN:
			//std::cout << "down\n";
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
			renderObject->getObjects()[MAPSIZE * MAPSIZE]->setPosition(glm::vec3(2.0f * fruitLocation[0] - 10.0f + 1, 2.0f * fruitLocation[1] - 10.0f + 1, 0.0f));
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
					renderObject->getObjects()[i * MAPSIZE + j]->setTextureElement(tailTexIndex);
				} else if (map[i][j] == headSize) {
					renderObject->getObjects()[i * MAPSIZE + j]->setTextureElement(headTexIndex);
				} else {
					renderObject->getObjects()[i * MAPSIZE + j]->setTextureElement(snakeTexIndex);
				}
				renderObject->getObjects()[i * MAPSIZE + j]->show = (bool)map[i][j];
			}
		}
	}
}







void genericCollision(GameObject* A, GameObject* B) {
	//std::cout << "[-Collision-]";
}

Render* AABBscene;
void AABBtest() {
	addToCollisionTable(GAMEOBJECT, GAMEOBJECT, genericCollision);

	AABBscene = new Render("armatureVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	AABBscene->root = new Node(new GameObject());
	AABBscene->root->getObject()->setPosition(glm::vec3(0.0f, 0.0f, -10.0f));


	int cubeMesh = AABBscene->addMesh("uvCube.txt", false, false);
	int monkeyMesh = AABBscene->addMesh("monkeySmoothNormal.txt", "monkeyHull.txt", false, false);

	Texture* cubeTex = new Texture("ohTheMisery.bmp", 0);
	int cubeTexIndex = AABBscene->addTexture(cubeTex);

	Texture* monkeyTex = new Texture("ohMyGah.bmp", 0);
	int monkeyTexIndex = AABBscene->addTexture(monkeyTex);

	int cubeIndex = AABBscene->addObject(
		new GameObject(
			glm::vec3(3.0, 3.0, 3.0),
			quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f),
			cubeMesh,
			cubeTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.5f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	AABBscene->root->addChild(new Node(AABBscene->getObjects()[cubeIndex]));

	int monkeyIndex = AABBscene->addObject(
		new GameObject(
			glm::vec3(0.3, 3.0, 3.0),
			quat(glm::vec3(1.0f, 0.0f, 0.0f), -3.1415f / 2.0f),
			monkeyMesh,
			monkeyTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	AABBscene->root->addChild(new Node(AABBscene->getObjects()[monkeyIndex]));




	// armature
	int bodyMesh = AABBscene->addMesh("body.txt", false, true);
	int bodyIndex = AABBscene->addObject(
		new GameObject(
			glm::vec3(0.0, 0.0, 0.0),
			quat(glm::vec3(0.707f, 0.707f, 0.0f), -3.1415f / 2.0f),
			bodyMesh,
			cubeTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -0.0f, 0.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	AABBscene->root->addChild(new Node(AABBscene->getObjects()[bodyIndex]));


	int animIndex = AABBscene->addAnimation("dabAnim.txt", false);
	AABBscene->playAnimation(animIndex, bodyIndex);

	addToRenderQueue(AABBscene);
}

void AABBtestUpdate(float deltaSec) {
	//std::cout << "obj 0\n";
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << AABBscene->getObjects()[0]->getAABBmin()[i];
	//	std::cout << ",";
	//}
	//std::cout << std::endl;
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << AABBscene->getObjects()[0]->getAABBmax()[i];
	//	std::cout << ",";
	//}
	//std::cout << std::endl;
}

enum ROOMS {
	OUTSIDE = 0,
	FRONTHALL,
	BATHROOM,
	LIVINGROOM,
	BACKHALL,
	TOPHALL,
	MASTERBED,
	NEARBED,
	FARBED,
	ESTABLISHING,
	KITCHEN,
	CUPBOARD,
	CLOSET,
	MOVE
};

enum SOUNDEFFECTS {
	STING = 0,
	ROOM,
	CRICKET,
	RADIO,
	FOOTSTEPS,
	SERENADE,
	PICKUP,
	PLANKREMOVE,
	LOCKED,
	UNLOCK,
	ANGRY
};

Render* firstLevel;
ROOMS activeCam = MOVE;
int hammerIndex;
int planksIndex;
int keyIndex;
int frontDoorIndex;
int masterBedDoorIndex;
int billBoardIndex;
int wellRestedTexIndex;
bool doorLocked = true;
bool frontLocked = true;

void plankCallback(void* data, void* other) {
	activeCam = (ROOMS)(int)other;
	firstLevel->addSAnimation(*(ScreenAnimation*)data);
	delete (ScreenAnimation*)data;
	firstLevel->getObjects()[planksIndex]->show = false;
}

void myGameInit() {
	addToCollisionTable(GAMEOBJECT, GAMEOBJECT, genericCollision);

	//firstLevel = new Render("defaultVertexShader.txt", "celShadingFragment.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->root = new Node(new GameObject());
	
	// outside = 0
	firstLevel->getCamera(OUTSIDE)->setPosition(glm::vec3(0.0f, 3.5f, 10.0f));
	firstLevel->getCamera(OUTSIDE)->setRotation(quat(glm::vec3(1.0, 0.0, 0.0), 0.0));

	// front hallway = 1
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(FRONTHALL)->setPosition(glm::vec3(5.0, 3.5, 3.5));
	firstLevel->getCamera(FRONTHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415 / 4.0));

	// bathroom = 2
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(BATHROOM)->setPosition(glm::vec3(-5.0, 3.5, 5.0));
	firstLevel->getCamera(BATHROOM)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 0.0 / 4.0));

	// living room = 3
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(LIVINGROOM)->setPosition(glm::vec3(-13.0, 3.5, 4.0));
	firstLevel->getCamera(LIVINGROOM)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 8.0));

	// back hallway = 4
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(BACKHALL)->setPosition(glm::vec3(-12.0, 3.5, -13.0));
	firstLevel->getCamera(BACKHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 1.5));

	// 2nd floor hallway = 5
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(TOPHALL)->setPosition(glm::vec3(3.0, 10.5, -15.0));
	firstLevel->getCamera(TOPHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 1.0) * quat(glm::vec3(1.0, 0.0, 0.0), -0.1));

	// master bedroom = 6
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(MASTERBED)->setPosition(glm::vec3(13.0, 10.5, -11.0));
	firstLevel->getCamera(MASTERBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -4.0 / 1.0) * quat(glm::vec3(1.0, 0.0, 0.0), -0.2));

	// near bedroom (closer to stairs) = 7
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(NEARBED)->setPosition(glm::vec3(-0.3, 10.5, -13.0));
	firstLevel->getCamera(NEARBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.5 / 2.0));

	// far bedroom = 8
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(FARBED)->setPosition(glm::vec3(-0.3, 10.5, -2.0));
	firstLevel->getCamera(FARBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.5 / 2.0));
	
	// outside establishing = 9
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(ESTABLISHING)->setPosition(glm::vec3(0.0, 3.5, 40.0));
	firstLevel->getCamera(ESTABLISHING)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 0.0) * quat(glm::vec3(1.0, 0.0, 0.0), 0.1));

	// outside establishing = 10
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(KITCHEN)->setPosition(glm::vec3(8.0, 3.5, -14.0));
	firstLevel->getCamera(KITCHEN)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.3));

	// outside establishing = 11
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(CUPBOARD)->setPosition(glm::vec3(4.0, 3.5, -14.0));
	firstLevel->getCamera(CUPBOARD)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415));

	// outside establishing = 12
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(CLOSET)->setPosition(glm::vec3(-4.0, 3.5, -14.0));
	firstLevel->getCamera(CLOSET)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415));

	// move-able camera = 13
	firstLevel->addCamera(new Camera(1.309f, WIDTH / HEIGHT, 0.1f, 100.0f));
	firstLevel->getCamera(MOVE)->setPosition(glm::vec3(0.0f, 3.5f, 10.0f));
	firstLevel->getCamera(MOVE)->setRotation(quat(glm::vec3(1.0, 0.0, 0.0), 0.0));

	activeCam = ESTABLISHING;

	// Lights
	firstLevel->addPointLight(glm::vec3(0.5, 5.0, 0.0), glm::vec4(1.0, 0.0, 0.0, 1.0)); // front hall
	firstLevel->addPointLight(glm::vec3(-3.0, 5.0, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0)); // bathroom
	firstLevel->addPointLight(glm::vec3(0.5, 5.0, -12.0), glm::vec4(1.0, 0.0, 0.0, 1.0)); // back hall
	firstLevel->addPointLight(glm::vec3(-0.0, 0.5, 24.0), glm::vec4(1.0, 2.0, 0.0, 1.0)); // outside
	firstLevel->addPointLight(glm::vec3(-0.0, 0.5, 16.0), glm::vec4(1.0, 2.0, 0.0, 1.0)); // outside closer
	firstLevel->addPointLight(glm::vec3(10.0, 5.0, 1.0), glm::vec4(0.8, 0.8, 0.4, 1.0)); // kitchen
	firstLevel->addPointLight(glm::vec3(4.5, 0.5, -9.0), glm::vec4(2.0, 1.0, 0.4, 1.0)); // cupboard
	firstLevel->addPointLight(glm::vec3(-3.5, 0.5, -9.0), glm::vec4(1.6, 1.0, 0.4, 1.0)); // closet
	firstLevel->addPointLight(glm::vec3(6.0, 10.0, -13.0), glm::vec4(0.9, 0.5, 0.2, 1.0)); // tophall
	firstLevel->addPointLight(glm::vec3(-3.5, 3.0, -12.0), glm::vec4(0.5, 0.5, 0.1, 1.0)); // planks
	//firstLevel->addPointLight(glm::vec3(0.5, 2.0, 0.0), glm::vec4(5.0, 5.0, 1.0, 1.0)); // super bright



	//int cubeMesh = firstLevel->addMesh("uvCube.txt", false, false);
	//Texture* cubeTex = new Texture("ohTheMisery.bmp", 0);
	//int cubeTexIndex = firstLevel->addTexture(cubeTex);

	//int cubeIndex = firstLevel->addObject(
	//	new GameObject(
	//		glm::vec3(3.0, 3.0, 7.0),
	//		quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f),
	//		cubeMesh,
	//		cubeTexIndex,
	//		glm::vec3(0.0f, 0.0f, 0.0f),
	//		glm::vec3(0.0f, 0.0f, 0.5f),
	//		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	//		true
	//	)
	//);
	//firstLevel->root->addChild(new Node(firstLevel->getObjects()[cubeIndex]));

	int houseMesh = firstLevel->addMesh("house.txt", false, false);
	//Texture* houseTex = new Texture("houseTex.bmp", 0);
	Texture* houseTex = new Texture("houseTexDirty.bmp", 0);
	int houseTexIndex = firstLevel->addTexture(houseTex);
	int houseIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(0.0, 0.0, -5.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			houseMesh,
			houseTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[houseIndex]));

	int floorMesh = firstLevel->addMesh("floor.txt", false, false);
	Texture* floorTex = new Texture("grassTextureBright.bmp", 0);
	int floorTexIndex = firstLevel->addTexture(floorTex);
	int floorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(0.0, -1.5, 0.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 3.1415 / 2.0),
			floorMesh,
			floorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[floorIndex]));

	int lightMesh = firstLevel->addMesh("light.txt", false, false); // front hall
	Texture* lightTex = new Texture("lightTex.bmp", 0);
	int lightTexIndex = firstLevel->addTexture(lightTex);
	int lightIndex1 = firstLevel->addObject(
		new GameObject(
			glm::vec3(0.5, 5.5, 0.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			lightMesh,
			lightTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[lightIndex1]));

	int lightIndex3 = firstLevel->addObject( // back hall
		new GameObject(
			glm::vec3(0.5, 5.5, -12.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			lightMesh,
			lightTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[lightIndex3]));

	int lightIndex4 = firstLevel->addObject( // kitchen
		new GameObject(
			glm::vec3(10.0, 5.75, 1.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			lightMesh,
			lightTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[lightIndex4]));

	int lightIndex5 = firstLevel->addObject( // cupboard
		new GameObject(
			glm::vec3(4.5, 4.5, -9.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			lightMesh,
			lightTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[lightIndex5]));

	int lightIndex6 = firstLevel->addObject( // closet
		new GameObject(
			glm::vec3(-3.5, 4.5, -9.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 0.0),
			lightMesh,
			lightTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[lightIndex6]));

	int keyMesh = firstLevel->addMesh("key.txt", false, false); // front hall
	Texture* keyTex = new Texture("keyTex.bmp", 0);
	int keyTexIndex = firstLevel->addTexture(keyTex);
	keyIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-3.5, 0.1, -9.0),
			quat(glm::vec3(1.0, 0.0, 0.0), 3.1415 / 2.0),
			keyMesh,
			keyTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[keyIndex]));

	int hammerMesh = firstLevel->addMesh("hammer.txt", false, false); // front hall
	Texture* hammerTex = new Texture("hammerTex.bmp", 0);
	int hammerTexIndex = firstLevel->addTexture(hammerTex);
	hammerIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(4.5, 0.1, -9.0),
			quat(glm::vec3(0.0, 1.0, 0.0), 3.1415 / 2.0) * quat(glm::vec3(1.0, 0.0, 0.0), 3.1415 / 2.0),
			hammerMesh,
			hammerTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[hammerIndex]));

	int doorMesh = firstLevel->addMesh("door.txt", false, false); // front hall
	Texture* doorTex = new Texture("doorTex.bmp", 0);
	int doorTexIndex = firstLevel->addTexture(doorTex);
	frontDoorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-0.5, 0.0, 4.5),
			quat(glm::vec3(0.0, 1.0, 0.0), 0.0 / 2.0),
			doorMesh,
			doorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[frontDoorIndex]));

	int bathroomDoorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-2.75, 0.0, 0.5),
			quat(glm::vec3(0.0, 1.0, 0.0), 3.1415 / 2.0),
			doorMesh,
			doorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[bathroomDoorIndex]));

	int nearBedDoorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-2.0, 7.5, -11.0),
			quat(glm::vec3(0.0, 1.0, 0.0), 3.3),
			doorMesh,
			doorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[nearBedDoorIndex]));

	int farBedDoorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-2.0, 7.5, -3.0),
			quat(glm::vec3(0.0, 1.0, 0.0), 3.3),
			doorMesh,
			doorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[farBedDoorIndex]));

	masterBedDoorIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(7.0, 7.5, -13.5),
			//quat(glm::vec3(0.0, 1.0, 0.0),  -0.7), // open
			quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 2.0),
			doorMesh,
			doorTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[masterBedDoorIndex]));


	int planksMesh = firstLevel->addMesh("planks.txt", false, false); // front hall
	Texture* planksTex = new Texture("planksDirty.bmp", 0);
	int planksTexIndex = firstLevel->addTexture(planksTex);
	planksIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-3.6, 0.0, -11.0),
			quat(glm::vec3(0.0, 1.0, 0.0), 3.1415),
			planksMesh,
			planksTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[planksIndex]));


	int billboardMesh = firstLevel->addMesh("billboard.txt", false, false); // front hall
	Texture* goToBedTex = new Texture("GO TO BED.bmp", 0);
	int goToBedTexIndex = firstLevel->addTexture(goToBedTex);
	Texture* wellRestedTex = new Texture("WELL-RESTED.bmp", 0);
	wellRestedTexIndex = firstLevel->addTexture(wellRestedTex);

	billBoardIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(-0.0, 6.0, 37.0), // go to bed
			//glm::vec3(0.0, 0.3, 32.0), // well rested
			quat(glm::vec3(0.0, 1.0, 0.0), 3.1415 / 2.0) * quat(glm::vec3(1.0, 0.0, 0.0), 3.1415 / 2.0),
			billboardMesh,
			goToBedTexIndex,
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1.0),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[billBoardIndex]));

	SoundSystem::system().loadSound("creepy sting.wav");
	SoundSystem::system().loadSound("room ambience.wav");
	SoundSystem::system().loadSound("night cricket.wav");
	SoundSystem::system().loadSound("radio.wav");
	SoundSystem::system().loadSound("footstepsDeep.wav");
	SoundSystem::system().loadSound("serenade.wav");
	SoundSystem::system().loadSound("pickup.wav");
	SoundSystem::system().loadSound("plankRemove.wav");
	SoundSystem::system().loadSound("locked.wav");
	SoundSystem::system().loadSound("unlock.wav");
	SoundSystem::system().loadSound("angry.wav");

	SoundSystem::system().playSound(RADIO, 0.01, LOOP);
	SoundSystem::system().playSound(ROOM, 0.2, LOOP);
	//SoundSystem::system().playSound(SERENADE, 0.1, LOOP);


	addToRenderQueue(firstLevel);
}
void transitionCallback(void* data, void* other) {
	activeCam = (ROOMS)(int) other;
	firstLevel->addSAnimation(*(ScreenAnimation*) data);
	delete (ScreenAnimation*) data;
}


void transition(ROOMS room) {
	// play sound
	SoundSystem::system().playSound(FOOTSTEPS, 0.5, ONCE);
	// fade in and out
	firstLevel->addSAnimation(ScreenAnimation{
		0.0, 0.8, FADEOUT, (void*)room, (void*) new ScreenAnimation{0.0, 0.2, DARK, (void*)room, (void*)new ScreenAnimation{0.0, 0.8, FADEIN, nullptr, nullptr, nullptr}, transitionCallback},
		transitionCallback
		});
}

void myGameManageClick(SDL_MouseButtonEvent mEvent) { // TODO detect if mouseDown
	// first check which room you are currently in
	switch (activeCam) {
		// different behavior depending on which room
	case OUTSIDE:
		// check mouse click range
		if (Button2D(glm::vec2(0.438, 0.365), glm::vec2(0.638, 0.964), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			if (frontLocked) {
				frontLocked = false;
				firstLevel->getObjects()[frontDoorIndex]->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 5.0 / 2.0));
				SoundSystem::system().playSound(UNLOCK, 0.5, ONCE);
			}
			transition(FRONTHALL);
		}
		break;
	case FRONTHALL:
		if (Button2D(glm::vec2(0.088, 0.176), glm::vec2(0.332, 0.865), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BATHROOM);
		} else if (Button2D(glm::vec2(0.662, 0.01), glm::vec2(0.999, 0.8), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(TOPHALL);
		} else if (Button2D(glm::vec2(0.464, 0.251), glm::vec2(0.691, 0.882), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BACKHALL);
		}
		break;
	case BATHROOM:
		if (Button2D(glm::vec2(0.82, 0.003), glm::vec2(0.995, 0.999), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(FRONTHALL);
		} else if (Button2D(glm::vec2(0.0, 0.0), glm::vec2(0.377, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(LIVINGROOM);
		}
		break;
	case LIVINGROOM:
		if (Button2D(glm::vec2(0.44, 0.397), glm::vec2(0.509, 0.614), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BACKHALL);
		} else if (Button2D(glm::vec2(0.868, 0.0), glm::vec2(1.0, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BATHROOM);
		}
		break;
	case BACKHALL:
		if (Button2D(glm::vec2(0.785937, 0.3375), glm::vec2(0.871875, 0.731944), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(LIVINGROOM);
		}
		else if (Button2D(glm::vec2(0.38125, 0.427778), glm::vec2(0.403906, 0.818056), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(CLOSET);
		}
		else if (Button2D(glm::vec2(0.338281, 0.470833), glm::vec2(0.35, 0.6625), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(CUPBOARD);
		}
		else if (Button2D(glm::vec2(0.353906, 0.309722), glm::vec2(0.369531, 0.722222), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(FRONTHALL);
		}
		else if (Button2D(glm::vec2(0.270313, 0.409722), glm::vec2(0.335938, 0.626389), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(KITCHEN);
		}
		break;
	case TOPHALL:
		if (Button2D(glm::vec2(0.0015625, 0.00416667), glm::vec2(0.2, 0.988889), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			if (firstLevel->getObjects()[keyIndex]->show == false) {
				if (doorLocked) {
					// unlock door
					SoundSystem::system().playSound(UNLOCK, 0.5, ONCE); // TODO unlock sound
					firstLevel->getObjects()[masterBedDoorIndex]->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -0.7));
					doorLocked = false;
				}
				else {
					transition(MASTERBED);
				}
			}
			else {
				// play locked sound
				SoundSystem::system().playSound(LOCKED, 0.5, ONCE); // TODO locked sound
			}
			
		}

		if (Button2D(glm::vec2(0.386719, 0.529167), glm::vec2(0.536719, 0.733333), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(FRONTHALL);
		} else if (Button2D(glm::vec2(0.932813, 0.148611), glm::vec2(0.996875, 0.998611), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(NEARBED);
		}
		else if (Button2D(glm::vec2(0.607031, 0.361111), glm::vec2(0.636719, 0.619444), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(FARBED);
		}
		break;
	case MASTERBED:
		if (Button2D(glm::vec2(0.116, 0.449), glm::vec2(0.319, 0.617), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			SoundSystem::system().playSound(STING, 0.4, ONCE);
			SoundSystem::system().playSound(SERENADE, 0.2, ONCE);
			// play sound
			// fade in and out
			firstLevel->addSAnimation(ScreenAnimation{
				0.0, 0.8, FADEOUT, (void*)ESTABLISHING, (void*) new ScreenAnimation{0.0, 4.0, DARK, (void*)ESTABLISHING, (void*)new ScreenAnimation{0.0, 0.8, FADEIN, nullptr, nullptr, nullptr}, transitionCallback},
				transitionCallback
				});
			firstLevel->getObjects()[billBoardIndex]->setPosition(glm::vec3(0.0, 0.3, 32.0)); // well rested billboard
			firstLevel->getObjects()[billBoardIndex]->setTextureElement(wellRestedTexIndex);
			firstLevel->getObjects()[billBoardIndex]->show = true;
		}







		if (Button2D(glm::vec2(0.820312, 0.00277778), glm::vec2(0.995313, 0.995833), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(TOPHALL);
		}
		break;
	case NEARBED:
		if (Button2D(glm::vec2(0.453125, 0.583333), glm::vec2(0.602344, 0.729167), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			SoundSystem::system().playSound(ANGRY, 0.5, ONCE);
			// TODO tell user it's not for you
		}


		if (Button2D(glm::vec2(0, 0.858333), glm::vec2(0.998437, 0.993056), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(TOPHALL);
		}
		break;
	case FARBED:
		if (Button2D(glm::vec2(0.34375, 0.584722), glm::vec2(0.511719, 0.718056), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			SoundSystem::system().playSound(ANGRY, 0.5, ONCE);
			// TODO tell user it's not for you
		}



		if (Button2D(glm::vec2(0, 0.858333), glm::vec2(0.998437, 0.993056), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(TOPHALL);
		}
		break;
	case ESTABLISHING:
		if (firstLevel->getObjects()[billBoardIndex]->show) {
			firstLevel->getObjects()[billBoardIndex]->show = false;			
			SoundSystem::system().playSound(PICKUP, 0.5, ONCE);
			break;
		}
		if (Button2D(glm::vec2(0.492188, 0.547222), glm::vec2(0.51875, 0.634722), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(OUTSIDE);

		}
		break;
	case KITCHEN:
		if (Button2D(glm::vec2(0.728, 0.0), glm::vec2(1.0, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(CUPBOARD);
		}
	case CUPBOARD:
		if (firstLevel->getObjects()[hammerIndex]->show == true && Button2D(glm::vec2(0.417969, 0.901389), glm::vec2(0.510938, 0.997222), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			firstLevel->getObjects()[hammerIndex]->show = false;
			SoundSystem::system().playSound(PICKUP, 0.5, ONCE);
		}





		if (Button2D(glm::vec2(0.773, 0.007), glm::vec2(1.0, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BACKHALL);
		}
		else if (Button2D(glm::vec2(0.0, 0.0), glm::vec2(0.164, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(KITCHEN);
		}
		break;
	case CLOSET:
		if (firstLevel->getObjects()[hammerIndex]->show == false && firstLevel->getObjects()[planksIndex]->show == true && Button2D(glm::vec2(0.178125, 0.277778), glm::vec2(0.695312, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			// remove planks
			
			SoundSystem::system().playSound(PLANKREMOVE, 0.5, ONCE);
			// fade in and out
			firstLevel->addSAnimation(ScreenAnimation{
				0.0, 0.8, FADEOUT, (void*)CLOSET, (void*) new ScreenAnimation{0.0, 1.0, DARK, (void*)CLOSET, (void*)new ScreenAnimation{0.0, 0.8, FADEIN, nullptr, nullptr, nullptr}, plankCallback},
				plankCallback
				});
		}
		else if (firstLevel->getObjects()[planksIndex]->show == false && Button2D(glm::vec2(0.417969, 0.901389), glm::vec2(0.510938, 0.997222), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			// pickup key
			firstLevel->getObjects()[keyIndex]->show = false;
			SoundSystem::system().playSound(PICKUP, 0.5, ONCE);
		}






		if (Button2D(glm::vec2(0.773, 0.007), glm::vec2(1.0, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			transition(BACKHALL);
		}
		else if (Button2D(glm::vec2(0.0, 0.0), glm::vec2(0.164, 1.0), false).has(glm::vec2(mEvent.x / WIDTH, mEvent.y / HEIGHT))) {
			//transition(FRONTHALL);
		}
		break;
	case MOVE:
		break;


	default:
		break;
	}



}


void myGameUpdate(float deltaSec) {
	if (lastKey.keysym.sym >= SDLK_0 && lastKey.keysym.sym <= SDLK_9) {
		activeCam = (ROOMS) (lastKey.keysym.sym - SDLK_0);
		lastKey.keysym.sym = NULL;
	}
	if (lastKey.type == SDL_KEYDOWN) {
		switch (lastKey.keysym.sym) {
		case SDLK_m:
			activeCam = MOVE;
			break;
		case SDLK_w:
			firstLevel->getCamera(MOVE)->move(glm::vec3(0.0, 0.0, -8 * deltaSec));
			break;
		case SDLK_a:
			firstLevel->getCamera(MOVE)->move(glm::vec3(-8 * deltaSec, 0.0, 0.0));
			break;
		case SDLK_s:
			firstLevel->getCamera(MOVE)->move(glm::vec3(0.0, 0.0, 8 * deltaSec));
			break;
		case SDLK_d:
			firstLevel->getCamera(MOVE)->move(glm::vec3(8 * deltaSec, 0.0, 0.0));
			break;
		case SDLK_SPACE:
			firstLevel->getCamera(MOVE)->move(glm::vec3(0.0, 8 * deltaSec, 0.0));
			break;
		case SDLK_LSHIFT:
			firstLevel->getCamera(MOVE)->move(glm::vec3(0.0, -8 * deltaSec, 0.0));
			break;
		case SDLK_LEFT:
			firstLevel->getCamera(MOVE)->rotate(quat(glm::vec3(0.0, 1.0, 0.0), 8 * deltaSec));
			break;
		case SDLK_RIGHT:
			firstLevel->getCamera(MOVE)->rotate(quat(glm::vec3(0.0, 1.0, 0.0), -8 * deltaSec));
			break;
		//case SDLK_UP:
		//	firstLevel->getCamera(MOVE)->rotate(quat(glm::vec3(1.0, 0.0, 0.0), 8 * deltaSec));
		//	break;
		//case SDLK_DOWN:
		//	firstLevel->getCamera(MOVE)->rotate(quat(glm::vec3(1.0, 0.0, 0.0), -8 * deltaSec));
			break;
		default:
			break;
		}
	}
	firstLevel->setCamera(activeCam);
}





// Below 3 are pre allocated functions
// for mouse down event
int mouseClick(SDL_MouseButtonEvent mEvent) {
	// first detect if it is pickup click, or move click
	std::cout << "glm::vec2(" << mEvent.x / WIDTH << ", " << mEvent.y / HEIGHT << "),\n"; // mouse position scaled by width and height

	myGameManageClick(mEvent);

	return 0;
}

int initialize() { // What should default initialize look like? 
	//std::cout << "user successfully performs initialization\n";

	//example();
	//exampleCel();
	//exampleAudio();
	//snakeInit();

	//AABBtest();
	myGameInit();

	return 0;
}

int globalUpdate(float deltaSec) {
	// TODO is this necessary?

	//snakeUpdate(deltaSec);
	//AABBtestUpdate(deltaSec);
	myGameUpdate(deltaSec);
	return 0;
}