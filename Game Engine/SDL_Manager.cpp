#include "SDL_Manager.h"
#include "Mesh.h"
#include "Instance.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>


SDL_Manager::SDL_Manager() {
	count = 0;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

SDL_Manager::~SDL_Manager() {
	SDL_Manager::closeWindow(0);
	SDL_Quit();
}

SDL_Manager& SDL_Manager::sdl() {
	static SDL_Manager instance;
	return instance;
}

void SDL_Manager::closeWindow(uint32_t id) {
	if (count == 0) {
		return;
	}
	// iterate over windows, using SDL_GetWindowID to find window
	for (int i = 0; i < count; ++i) {
		if (SDL_GetWindowID(windows[i]) == id) {
			// If OpenGL window
			if (i == 0) {
				SDL_GL_DeleteContext(context);
				// Delete every window
				while (count != 0) {
					SDL_DestroyWindow(windows[count - 1]);
					windows[count - 1] = nullptr;
					count--;
				}
				// Manually issue quit event
				SDL_Event e;
				e.type = SDL_QUIT;
				SDL_PushEvent(&e);
				return;
			}
			// Otherwise:

			// Swap
			std::swap(buffers[i], buffers[count - 1]);
			std::swap(windows[i], windows[count - 1]);
			// n pop
			SDL_DestroyWindow(windows[count - 1]);
			windows[count - 1] = nullptr;
			count--;
		}
	}
}

void SDL_Manager::spawnWindow(const char* title, int width, int height, SDL_bool resizable) {
	if (count >= MAXWINDOWS) {
		// Max windows reached
		std::cout << "max windows already reached\n";
		return;
	}
	if (count == 0) { // If first window
		windows[count] = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL);
		context = SDL_GL_CreateContext(windows[count]);

		// TODO Google what all these do
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);


		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);


		// initialize glew
		glewExperimental = GL_TRUE;
		GLenum glewerror = glewInit();
		if (glewerror != GLEW_OK) {
			//bad times..
			std::cout << "glew error!\n";
			throw;
		}

	}
	else {
		windows[count] = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
	}
	if (windows[count] == NULL) {
		// error
		std::cout << "window creation unsuccessful\n";
		return;
	}
	SDL_SetWindowResizable(windows[count], resizable);

	buffers[count] = SDL_GetWindowSurface(windows[count]);


	count++;
}

void SDL_Manager::handleResize(uint32_t id) {
	for (int i = 0; i < count; i++) {
		if (SDL_GetWindowID(windows[i]) == id) {
			buffers[i] = SDL_GetWindowSurface(windows[i]);
		}
	}
}

extern GLuint program;
extern GLint uniformIndexProj;

extern Instance* cube;

void SDL_Manager::updateWindows() {
	for (size_t i = 0; i < count; i++) {
		// GL window
		if (i == 0) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(cube->mesh->getVAO());
			glUseProgram(program);
			glm::mat4 proj = glm::perspective(1.309f, 16.0f / 9.0f, 0.1f, 100.0f);
			glUniformMatrix4fv(uniformIndexProj, 1, GL_FALSE, glm::value_ptr(proj)); // TODO: there's gotta be a better way than just every frame



			glDrawArrays(GL_TRIANGLES, 0, cube->mesh->getVertexCount());
			glBindVertexArray(0);
			glUseProgram(0);
			SDL_GL_SwapWindow(windows[i]);
		} else {
			// Rest of the windows
			SDL_UpdateWindowSurface(windows[i]);
		}
	}
}