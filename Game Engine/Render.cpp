 #include "Render.h"
#include "Utility.h"
#include "RenderInfo.h"

#include <iostream>
#include <string>
#include <glm/ext.hpp>
#include <fstream>


int Render::loadShaders(std::string vertFile, std::string fragFile) {
	const GLchar* vertShader;
	const GLchar* fragShader;

	GLuint vert;
	GLuint frag;

	std::ifstream inputVert(vertFile);
	std::string vertCode(std::istreambuf_iterator<char>{inputVert}, {});
	vertShader = (GLchar*)vertCode.data();

	std::ifstream inputFrag(fragFile);
	std::string fragCode(std::istreambuf_iterator<char>{inputFrag}, {});
	fragShader = (GLchar*)fragCode.data();


	// Compile shaders
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShader, NULL);

	glCompileShader(vert);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		std::cout << "Vertex Shader:\n";
		std::cout << infoLog;
	}
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShader, NULL);

	glCompileShader(frag);
	// check for shader compile errors
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "Fragment Shader:\n";
		std::cout << infoLog;
	}

	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Combined\n";
		std::cout << infoLog;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	glUseProgram(0);
	uniformIndexProj = glGetUniformLocation(program, "proj");
	uniformIndexTran = glGetUniformLocation(program, "tran");
	uniformIndexColor = glGetUniformLocation(program, "baseColor");
	return 0;
}

// SHOULD BE CALLED EVERYTIME MESH IS CHANGED, GAME OBJECT POSITION/ROTATION IS CHANGED
void Render::assignAABB(GameObject* obj) {
	// grab mesh boundary points
	const glm::vec3* boundaryPoints = meshes[obj->getRenderElement()]->getBoundaryPoints();
	glm::vec3 boundaryTransformed[8];
	quat rot = obj->getRotation();
	// apply transformation state to 8 points
	for (int i = 0; i < 8; ++i) {
		boundaryTransformed[i] = rot * boundaryPoints[i];
	}
	// get min and max from transformed set
	glm::vec3 min = boundaryTransformed[0];
	glm::vec3 max = boundaryTransformed[0];

	for (int i = 0; i < 8; ++i) {
		for (int d = 0; d < 3; ++d) {
			min[d] = std::min(boundaryTransformed[i][d], min[d]);
			max[d] = std::max(boundaryTransformed[i][d], max[d]);
		}
	}

	// offset point by position
	min += obj->getPosition();
	max += obj->getPosition();

	obj->setAABBmin(min);
	obj->setAABBmax(max);
}

int Render::addMesh(std::string fileName, bool bytes) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	std::vector<float>data = {};
	meshes[meshCount] = new Mesh(parseMesh(fileName, data, bytes), data);
	physicsMeshes[meshCount] = meshes[meshCount];
	//std::cout << fileName << std::endl;
	//for (int i = 0; i < 8; ++i) {
	//	std::cout << "\t";
	//	std::cout << meshes[meshCount]->getBoundaryPoints()[i][0] << ",\t";
	//	std::cout << meshes[meshCount]->getBoundaryPoints()[i][1] << ",\t";
	//	std::cout << meshes[meshCount]->getBoundaryPoints()[i][2] << std::endl;
	//}
	return meshCount++;
}

int Render::addMesh(Mesh* inputMesh) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	meshes[meshCount] = inputMesh;
	physicsMeshes[meshCount] = inputMesh;
	return meshCount++;
}

int Render::addMesh(Mesh* inputMesh, Mesh* inputPhysicsMesh) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	meshes[meshCount] = inputMesh;
	physicsMeshes[meshCount] = inputPhysicsMesh;
	return meshCount++;
}
int Render::addMesh(std::string fileName, std::string physicsFileName, bool bytes) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	std::vector<float>data = {};
	meshes[meshCount] = new Mesh(parseMesh(fileName, data, bytes), data);
	data.clear();
	physicsMeshes[meshCount] = new Mesh(parseMesh(physicsFileName, data, bytes), data);

	return meshCount++;
}

int Render::addTexture(std::string fileName) {
	if (textureCount >= MAXTEXTURES) {
		return -1;
	}
	// run parse texture function TODO
	std::cout << "incomplete" << std::endl;

	return textureCount++;
}

int Render::addTexture(Texture* inputTexture) {
	if (textureCount >= MAXTEXTURES) {
		return -1;
	}
	textures[textureCount] = inputTexture;

	return textureCount++;
}

int Render::addObject(GameObject* obj) {
	if (objectCount >= MAXOBJECTS) {
		return -1;
	}
	objects[objectCount] = obj;


	return objectCount++;
}

void draw(RenderInfo info, GameObject* object) {
	if (object->show) {
		glBindVertexArray((info.meshes[object->getRenderElement()])->getVAO());
		glActiveTexture(GL_TEXTURE0); // TODO may be unneccessary
		if (object->getTextureElement() != -1) {
			glBindTexture(GL_TEXTURE_2D, (info.textures[object->getTextureElement()])->getTBO());
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glUniformMatrix4fv(info.uniformIndexTran, 1, GL_FALSE, glm::value_ptr(object->getParentTransform() * object->getModel()));
		glUniform4fv(info.uniformIndexColor, 1, glm::value_ptr(object->getColor()));
		glDrawArrays(GL_TRIANGLES, 0, (info.meshes[object->getRenderElement()])->getVertexCount());
	}
}

void Render::update(float delta) {
	// Update Objects
	for (int i = 0; i < objectCount; ++i) {
		// use old parent transform
		removeFromGrid(objects[i]);
		objects[i]->update(delta);
	}

	RenderInfo info = { uniformIndexProj, uniformIndexTran, uniformIndexColor, textures, meshes };
	glUseProgram(program);
	glUniformMatrix4fv(uniformIndexProj, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
	//for (int i = 0; i < objectCount; ++i) {
	//	draw(info, objects[i], glm::mat4(1));
	//}
	// Render objects using tree
	root->render(info, camera, glm::mat4(1), draw); // this updates parent transform

	for (int i = 0; i < objectCount; ++i) {
		assignAABB(objects[i]);
		addToGrid(objects[i]);
	}
	generateCollisionSet();
	// loop through set
	// perform GJK
	// if colliding, then perform user defined signature
	std::set<std::pair<GameObject*, GameObject*>>::iterator iter;
	//std::cout << collisionSet.size() << std::endl;
	glm::mat4 firstTran;
	glm::mat4 secondTran;
	for (iter = collisionSet.begin(); iter != collisionSet.end(); ++iter) {
		firstTran = iter->first->getParentTransform();
		secondTran = iter->second->getParentTransform();
		if (GJK(
			physicsMeshes[iter->first->getRenderElement()],
			iter->first->getRotation(),
			iter->first->getPosition(),
			physicsMeshes[iter->second->getRenderElement()],
			iter->second->getRotation(),
			iter->second->getPosition()
		)) {
			std::cout << "yay intersect!\n";
			// TODO apply func table here
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
}
void Render::initializeGrid() {
	// produce min and max
	// intialize obj count to 0
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			for (int k = 0; k < GRIDSIZE; ++k) {
				collisionGrid[i][j][k].objCount = 0;
				collisionGrid[i][j][k].min = glm::vec3(
					i * TILESIZE + WORLDMIN,
					j * TILESIZE + WORLDMIN,
					k * TILESIZE + WORLDMIN
				);
				collisionGrid[i][j][k].max = glm::vec3(
					i * TILESIZE + TILESIZE + WORLDMIN,
					j * TILESIZE + TILESIZE + WORLDMIN,
					k * TILESIZE + TILESIZE + WORLDMIN
				);

			}
		}
	}
}
void Render::addToGrid(GameObject* obj) { // TODO this code is not DRY (along with removeFromGrid())
	// based on position and size
	// based on WORLDMIN, WORLDMAX, and GRIDSIZE
	int minI, minJ, minK, maxI, maxJ, maxK;
	glm::vec3 objMin = obj->getAABBmin() + glm::vec3(obj->getParentTransform()[3]);
	glm::vec3 objMax = obj->getAABBmax() + glm::vec3(obj->getParentTransform()[3]);
	minI = std::max(static_cast<int>(floor((objMin.x - WORLDMIN) / TILESIZE)), 0);
	maxI = std::min(static_cast<int>(ceil((objMax.x - WORLDMIN) / TILESIZE)), GRIDSIZE);
	minJ = std::max(static_cast<int>(floor((objMin.y - WORLDMIN) / TILESIZE)), 0);
	maxJ = std::min(static_cast<int>(ceil((objMax.y - WORLDMIN) / TILESIZE)), GRIDSIZE);
	minK = std::max(static_cast<int>(floor((objMin.z - WORLDMIN) / TILESIZE)), 0);
	maxK = std::min(static_cast<int>(ceil((objMax.z - WORLDMIN) / TILESIZE)), GRIDSIZE);

	// These are tiles object occupies (according to AABB)
	for (int i = minI; i < maxI; ++i) {
		for (int j = minJ; j < maxJ; ++j) {
			for (int k = minK; k < maxK; ++k) {
				if (collisionGrid[i][j][k].objCount < MAXOBJECTSPERTILE) {
					collisionGrid[i][j][k].obj[collisionGrid[i][j][k].objCount] = obj;
					++collisionGrid[i][j][k].objCount;
				}
				else {
					std::cout << "TOO MANY OBJECTS IN ONE TILE!\n";
				}
			}
		}
	}


}
void Render::removeFromGrid(GameObject* obj) {
	int minI, minJ, minK, maxI, maxJ, maxK;
	glm::vec3 objMin = obj->getAABBmin() + glm::vec3(obj->getParentTransform()[3]);
	glm::vec3 objMax = obj->getAABBmax() + glm::vec3(obj->getParentTransform()[3]);
	minI = std::max(static_cast<int>(floor((objMin.x - WORLDMIN) / TILESIZE)), 0);
	maxI = std::min(static_cast<int>(ceil((objMax.x - WORLDMIN) / TILESIZE)), GRIDSIZE);
	minJ = std::max(static_cast<int>(floor((objMin.y - WORLDMIN) / TILESIZE)), 0);
	maxJ = std::min(static_cast<int>(ceil((objMax.y - WORLDMIN) / TILESIZE)), GRIDSIZE);
	minK = std::max(static_cast<int>(floor((objMin.z - WORLDMIN) / TILESIZE)), 0);
	maxK = std::min(static_cast<int>(ceil((objMax.z - WORLDMIN) / TILESIZE)), GRIDSIZE);

	// These are tiles object occupies (according to AABB)
	for (int i = minI; i < maxI; ++i) {
		for (int j = minJ; j < maxJ; ++j) {
			for (int k = minK; k < maxK; ++k) {
				// remove from tile
				for (int objI = 0; objI < collisionGrid[i][j][k].objCount; ++objI) {
					if (collisionGrid[i][j][k].obj[objI] == obj) {
						--collisionGrid[i][j][k].objCount;
						std::swap(collisionGrid[i][j][k].obj[objI], collisionGrid[i][j][k].obj[collisionGrid[i][j][k].objCount]);
					}
				}
			}
		}
	}
}
void Render::generateCollisionSet() {
	collisionSet.clear();
	// loop through tiles
	// this should produce pairs of objects to perform GJK on
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			for (int k = 0; k < GRIDSIZE; ++k) {
				// loop through pairs of objects in tile
				for (int l = 0; l < collisionGrid[i][j][k].objCount; ++l) {
					for (int m = l + 1; m < collisionGrid[i][j][k].objCount; ++m) {
						// insert pair in collision set
						if (collisionGrid[i][j][k].obj[l] < collisionGrid[i][j][k].obj[m]) {
							collisionSet.insert(std::make_pair(
								collisionGrid[i][j][k].obj[l],
								collisionGrid[i][j][k].obj[m]
							));
						} else {
							collisionSet.insert(std::make_pair(
								collisionGrid[i][j][k].obj[m],
								collisionGrid[i][j][k].obj[l]
							));
						}
					}
				}
			}
		}
	}
}

Render::Render(std::string vertFile, std::string fragFile, Camera* cameraIn) {
	if (loadShaders(vertFile, fragFile)) {
		std::cout << "Shaders didn't load correctly\n";
		throw;
	}
	objectCount = 0;
	meshCount = 0;
	textureCount = 0;
	camera = cameraIn;
	root = nullptr;
	initializeGrid();
}

Render::~Render() {
	for (int i = 0; i < meshCount; ++i) {
		delete meshes[i];
		delete physicsMeshes[i];
	}
	for (int i = 0; i < objectCount; ++i) {
		delete objects[i];
	}
	for (int i = 0; i < textureCount; ++i) {
		delete textures[i];
	}
	delete camera;
	delete root;
}

GameObject** Render::getObjects() {
	return objects;
}

Camera* Render::getCamera() {
	return camera;
}

Mesh** Render::getMeshes() {
	return meshes;
}

int addToRenderQueue(Render* renderObject) {
	RenderQueue[renderCount] = renderObject;
	return renderCount++;
}