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

// Returns error value
SDL_Surface* parseTexture(std::string fileName) {
	return SDL_LoadBMP(fileName.c_str());
}


// Returns triangle count, takes in file name and vector for vertices and normals
size_t parseMesh(std::string filename, std::vector<float>& data, bool bytes) {
	int triangleCount = 0;
	std::fstream input(filename);
	if (bytes) {
		float value;
		if (input.is_open()) {
			// triangle count
			input.read((char*)&triangleCount, 4);
			for (int i = 0; i < 24 * triangleCount; ++i) { // 18 if just pos and norm
				input.read((char*)&value, 4);
				data.push_back(value);
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
		}
	}
	return (size_t)triangleCount;
}