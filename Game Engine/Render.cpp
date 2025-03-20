 #include "Render.h"
#include "Utility.h"

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

int Render::addMesh(std::string fileName, bool bytes) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	std::vector<float>data = {};
	meshes[meshCount] = new Mesh(parseMesh(fileName, data, bytes), data);
	//std::cout << meshes[meshCount]->getVertexCount();
	return meshCount++;
}

int Render::addMesh(Mesh* inputMesh) {
	if (meshCount >= MAXMESHES) {
		return -1;
	}
	meshes[meshCount] = inputMesh;
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

void Render::update(float delta) {
	glUseProgram(program);
	glUniformMatrix4fv(uniformIndexProj, 1, GL_FALSE, glm::value_ptr(perspective));	
	for (int i = 0; i < objectCount; ++i) {
		objects[i]->update(delta);
		glBindVertexArray((meshes[objects[i]->getRenderElement()])->getVAO());
		glActiveTexture(GL_TEXTURE0); // TODO may be unneccessary
		if (objects[i]->getTextureElement() != -1) {
			glBindTexture(GL_TEXTURE_2D, (textures[objects[i]->getTextureElement()])->getTBO());
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glUniformMatrix4fv(uniformIndexTran, 1, GL_FALSE, glm::value_ptr(objects[i]->getModel()));
		glUniform4fv(uniformIndexColor, 1, glm::value_ptr(objects[i]->getColor()));
		//TODO bind texture
		if (objects[i]->show) {
			glDrawArrays(GL_TRIANGLES, 0, (meshes[objects[i]->getRenderElement()])->getVertexCount());
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

Render::Render(std::string vertFile, std::string fragFile, glm::mat4 proj) {
	if (loadShaders(vertFile, fragFile)) {
		std::cout << "Shaders didn't load correctly\n";
		throw;
	}
	objectCount = 0;
	meshCount = 0;
	textureCount = 0;
	perspective = proj; // TODO send perspective matrix over
}

Render::~Render() {
	for (int i = 0; i < meshCount; ++i) {
		delete meshes[i];
	}
	for (int i = 0; i < objectCount; ++i) {
		delete objects[i];
	}
	for (int i = 0; i < textureCount; ++i) {
		delete textures[i];
	}
}