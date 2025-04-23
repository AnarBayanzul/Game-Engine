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

	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
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
	RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
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
	Render* renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
	
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
	//RenderCel* renderObject = new RenderCel("defaultVertexShader.txt", "celShadingFragment.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
	renderObject = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
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
	std::cout << "[-Collision-]";
}

Render* AABBscene;
void AABBtest() {
	addToCollisionTable(GAMEOBJECT, GAMEOBJECT, genericCollision);

	AABBscene = new Render("armatureVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, 1.0f, 0.1f, 100.0f));
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
	CLOSET
};

Render* firstLevel;
void myGameInit() {
	addToCollisionTable(GAMEOBJECT, GAMEOBJECT, genericCollision);

	firstLevel = new Render("defaultVertexShader.txt", "defaultFragmentShader.txt", new Camera(1.309f, 1280.0/720.0, 0.1f, 100.0f));
	firstLevel->root = new Node(new GameObject());
	
	// outside = 0
	firstLevel->getCamera(OUTSIDE)->setPosition(glm::vec3(0.0f, 3.5f, 10.0f));
	firstLevel->getCamera(OUTSIDE)->setRotation(quat(glm::vec3(1.0, 0.0, 0.0), 0.0));

	// front hallway = 1
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(FRONTHALL)->setPosition(glm::vec3(5.0, 3.5, 3.5));
	firstLevel->getCamera(FRONTHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415 / 4.0));

	// bathroom = 2
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(BATHROOM)->setPosition(glm::vec3(-5.0, 3.5, 5.0));
	firstLevel->getCamera(BATHROOM)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 0.0 / 4.0));

	// living room = 3
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(LIVINGROOM)->setPosition(glm::vec3(-13.0, 3.5, 4.0));
	firstLevel->getCamera(LIVINGROOM)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 8.0));

	// back hallway = 4
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(BACKHALL)->setPosition(glm::vec3(-12.0, 3.5, -13.0));
	firstLevel->getCamera(BACKHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 1.5));

	// 2nd floor hallway = 5
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(TOPHALL)->setPosition(glm::vec3(3.0, 10.5, -15.0));
	firstLevel->getCamera(TOPHALL)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -3.1415 / 1.0) * quat(glm::vec3(1.0, 0.0, 0.0), -0.1));

	// master bedroom = 6
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(MASTERBED)->setPosition(glm::vec3(13.0, 10.5, -11.0));
	firstLevel->getCamera(MASTERBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), -4.0 / 1.0) * quat(glm::vec3(1.0, 0.0, 0.0), -0.2));

	// near bedroom (closer to stairs) = 7
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(NEARBED)->setPosition(glm::vec3(-0.3, 10.5, -13.0));
	firstLevel->getCamera(NEARBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.5 / 2.0));

	// far bedroom = 8
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(FARBED)->setPosition(glm::vec3(-0.3, 10.5, -2.0));
	firstLevel->getCamera(FARBED)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.5 / 2.0));
	
	// outside establishing = 9
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(ESTABLISHING)->setPosition(glm::vec3(0.0, 3.5, 20.0));
	firstLevel->getCamera(ESTABLISHING)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 0.0) * quat(glm::vec3(1.0, 0.0, 0.0), 0.1));

	// outside establishing = 10
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(KITCHEN)->setPosition(glm::vec3(8.0, 3.5, -14.0));
	firstLevel->getCamera(KITCHEN)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.3));

	// outside establishing = 11
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(CUPBOARD)->setPosition(glm::vec3(4.0, 3.5, -14.0));
	firstLevel->getCamera(CUPBOARD)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415));

	// outside establishing = 12
	firstLevel->addCamera(new Camera(1.309f, 1280.0 / 720.0, 0.1f, 100.0f));
	firstLevel->getCamera(CLOSET)->setPosition(glm::vec3(-4.0, 3.5, -14.0));
	firstLevel->getCamera(CLOSET)->setRotation(quat(glm::vec3(0.0, 1.0, 0.0), 3.1415));

	firstLevel->setCamera(KITCHEN);

	int cubeMesh = firstLevel->addMesh("uvCube.txt", false, false);
	Texture* cubeTex = new Texture("ohTheMisery.bmp", 0);
	int cubeTexIndex = firstLevel->addTexture(cubeTex);

	int cubeIndex = firstLevel->addObject(
		new GameObject(
			glm::vec3(3.0, 3.0, 7.0),
			quat(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f),
			cubeMesh,
			cubeTexIndex,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.5f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			true
		)
	);
	firstLevel->root->addChild(new Node(firstLevel->getObjects()[cubeIndex]));

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

	addToRenderQueue(firstLevel);
}

void myGameUpdate(float deltaSec) {
	if (lastKey.keysym.sym >= SDLK_0 && lastKey.keysym.sym <= SDLK_9) {
		firstLevel->setCamera(lastKey.keysym.sym - SDLK_0);
	}
	switch (lastKey.keysym.sym) {
	case SDLK_0:
		//std::cout << "left\n";
		break;
	case SDLK_1:
		//std::cout << "right\n";
		break;
	case SDLK_2:
		//std::cout << "up\n";
		break;
	case SDLK_3:
		//std::cout << "down\n";
		break;
	case SDLK_4:
		//std::cout << "down\n";
		break;
	case SDLK_5:
		//std::cout << "down\n";
		break;
	case SDLK_6:
		//std::cout << "down\n";
		break;
	default:
		break;
	}
}

int mouseClick(SDL_MouseButtonEvent mEvent) {
	// first detect if it is pickup click, or move click
	std::cout << mEvent.x << ", " << mEvent.y << "\n";
	std::cout << mEvent.state << "\n";
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