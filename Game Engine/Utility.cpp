#include "Utility.h"

#include "Mesh.h"
#include "GameObject.h"

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_surface.h>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>


size_t delta;
float deltaSec;

Render* RenderQueue[MAXRENDEROBJECTS];
int renderCount = 0;

// Returns error value
SDL_Surface* parseTexture(std::string fileName) {
	//SDL_Surface* example = SDL_LoadBMP(fileName.c_str());

	// ohTheMisery is BGR24
	// ohMyGah is ARGB8888

	//std::cout << "Image format\n";
	//std::cout << std::hex << example->format->format << std::endl;
	//std::cout << (GLuint)example->format->BytesPerPixel << std::endl;

	return SDL_LoadBMP(fileName.c_str());
}


// Returns triangle count, takes in file name and vector for vertices and normals
size_t parseMesh(std::string fileName, std::vector<float>& data, std::vector<vertexBone>& vertexBoneData, std::vector<boneData>& bones, bool bytes, bool boned) {
	int triangleCount = 0;
	int boneCount = 0;
	std::fstream input(fileName);
	if (bytes) {
		float value;
		int valueInt;
		if (input.is_open()) {
			// triangle count
			input.read((char*)&triangleCount, 4);
			for (int i = 0; i < 24 * triangleCount; ++i) { // 18 if just pos and norm
				input.read((char*)&value, 4);
				data.push_back(value);
			}
			if (boned) {
				vertexBone vBone;
				boneData bData;
				// now read bone indices and weight
				for (int i = 0; i < 9 * triangleCount; ++i) {
					for (int j = 0; j < 4; ++j) {
						input.read((char*)&valueInt, 4);
						vBone.index[j] = valueInt;
					}
					for (int j = 0; j < 4; ++j) {
						input.read((char*)&value, 4);
						vBone.weight[j] = value;
					}
					vertexBoneData.push_back(vBone);
				}
				// then read bones
				input.read((char*)&boneCount, 4);
				for (int i = 0; i < boneCount; ++i) {
					for (int j = 0; j < 3; ++j) {
						input.read((char*)&value, 4);
						bData.origin[j] = value;
					}
					for (int j = 0; j < 3; ++j) {
						input.read((char*)&value, 4);
						bData.parent_to_this[j] = value;
					}
					input.read((char*)&valueInt, 4);
					bData.parent = valueInt;

					bones.push_back(bData);
				}
			}
		}
	}
	else {
		char delimiter = ' ';
		if (input.is_open()) {
			std::string line;
			// triangle count
			std::getline(input, line, delimiter);
			triangleCount = std::stoi(line);
			// Rest of the lines
			for (int i = 0; i < 24 * triangleCount; ++i) { // 18 if just pos and norm
				std::getline(input, line, delimiter);
				data.push_back(std::stof(line));
			}
			float total;
			if (boned) {
				vertexBone vBone;
				boneData bData;
				// now read bone indices and weight
				for (int i = 0; i < 3 * triangleCount; ++i) {
					for (int j = 0; j < 4; ++j) {
						std::getline(input, line, delimiter);
						vBone.index[j] = std::stoi(line);
					}
					total = 0;
					for (int j = 0; j < 4; ++j) {
						std::getline(input, line, delimiter);
						vBone.weight[j] = std::stof(line);
						total = total + vBone.weight[j];
					}
					for (int j = 0; j < 4; ++j) {
						vBone.weight[j] = vBone.weight[j] / total;
					}
					vertexBoneData.push_back(vBone);
				}
				// then read bones
				std::getline(input, line, delimiter);
				boneCount = std::stoi(line);
				for (int i = 0; i < boneCount; ++i) {
					for (int j = 0; j < 3; ++j) {
						std::getline(input, line, delimiter);
						bData.origin[j] = std::stof(line);
					}
					bones.push_back(bData);
				}
				for (int i = 0; i < boneCount; ++i) {
					for (int j = 0; j < 3; ++j) {
						std::getline(input, line, delimiter);
						bones[i].parent_to_this[j] = std::stof(line);
					}
				}
				for (int i = 0; i < boneCount; ++i) {
					std::getline(input, line, delimiter);
					bones[i].parent = std::stoi(line);
					
				}
			}

		}
	}
	return (size_t)triangleCount;
}



size_t parseAnim(std::string fileName, int& boneCount, std::vector<Keyframe>& data, bool bytes) {
	int keyCount = 0;
	std::fstream input(fileName);
	if (bytes) {
		std::cout << "byte files for anim don't work yet :'(\n";
		float value;
		int valueInt;
		if (input.is_open()) {
			input.read((char*)&value, 4);
		}
	} else {
		char delimiter = ' ';
		Keyframe thisKey;
		if (input.is_open()) {
			std::string line;
			// keycount
			std::getline(input, line, delimiter);
			keyCount = std::stoi(line);
			// boneCount
			std::getline(input, line, delimiter);
			boneCount = std::stoi(line);


			// Rest of the lines
			int ID;
			glm::vec4 rot;
			for (int i = 0; i < keyCount; ++i) { // 18 if just pos and norm
				thisKey = Keyframe();
				// timestamp
				std::getline(input, line, delimiter);
				thisKey.timeStamp = std::stof(line);

				for (int j = 0; j < boneCount; ++j) {
					// bone ID
					std::getline(input, line, delimiter);
					ID = std::stoi(line);
					// quaternion
					for (int k = 0; k < 4; ++k) {
						std::getline(input, line, delimiter);
						rot[k] = std::stof(line);
					}
					thisKey.boneTransform.push_back(std::make_pair(ID, quat(rot)));
				}
				data.push_back(thisKey);
			}
		}
	}
	return keyCount;
}