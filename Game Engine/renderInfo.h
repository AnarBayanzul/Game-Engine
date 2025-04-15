#pragma once
#include "Texture.h"
#include "Mesh.h"

#include <SDL_opengl.h>

struct RenderInfo {
	GLint uniformIndexProj;
	GLint uniformIndexTran;
	GLint uniformIndexColor;
	GLint uniformIndexBones;
	Texture** textures;
	Mesh** meshes;
};