#pragma once
#include <string>
#include <vector>
// For external global functions, the API
// Intention is for source file to remain hidden
// Including additional libraries for the user is useful here

// Declared in Application.cpp
void initialize();

int parseMesh(std::string filename, std::vector<float>& data, bool bytes);

int getDeltaTime();
float getDeltaSeconds();