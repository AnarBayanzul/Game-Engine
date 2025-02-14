#include "Engine.h"

#include <string>
#include <vector>
#include <fstream>

// Returns triangle count, takes in file name and vector for vertices and normals
int parseMesh(std::string filename, std::vector<float>& data, bool bytes) {
	int triangleCount = 0;
	std::fstream input(filename);
	if (bytes) {
		float value;
		if (input.is_open()) {
			// triangle count
			input.read((char*)&triangleCount, 4);
			for (int i = 0; i < 18 * triangleCount; ++i) {
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
			for (int i = 0; i < 18 * triangleCount; ++i) {
				std::getline(input, line, delimiter);
				data.push_back(std::stof(line));
			}
		}
	}
	return triangleCount;
}

int getDeltaTime() {
	return 0;
}

float getDeltaSeconds() {
	return 0.0f;
}