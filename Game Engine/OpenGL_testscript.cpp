//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <iostream>
//
//int main(int argc, char** argv) {
//	SDL_Init(SDL_INIT_VIDEO);
//
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//	SDL_Window* window = SDL_CreateWindow("OpenGL Testing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL);
//	SDL_GLContext context = SDL_GL_CreateContext(window);
//
//	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
//
//	
//
//	bool exit = false;
//	SDL_Event e;
//	while (!exit) {
//
//
//
//
//		GLenum err=0;//Example OpenGL errors
//		if ((err = glGetError()) != GL_NO_ERROR) {
//			switch (err) {
//			case GL_INVALID_ENUM:
//				std::cout << "a" << std::endl;
//					break;
//			case GL_INVALID_VALUE:
//				std::cout << "b" << std::endl;
//				break;
//			case GL_INVALID_OPERATION:
//				std::cout << "c" << std::endl;
//				break;
//			case GL_INVALID_FRAMEBUFFER_OPERATION:
//				std::cout << "d" << std::endl;
//				break;
//			case GL_OUT_OF_MEMORY:
//				std::cout << "e" << std::endl;
//				break;
//			case GL_STACK_UNDERFLOW:
//				std::cout << "f" << std::endl;
//				break;
//			case GL_STACK_OVERFLOW:
//				std::cout << "g" << std::endl;
//				break;
//				}
//			//std::cout << err << std:: endl;
//
//			}
//
//
//
//
//		while (SDL_PollEvent(&e)) {
//			switch (e.type) {
//			case SDL_QUIT:
//				exit = true;
//				break;
//			}
//		}
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		SDL_GL_SwapWindow(window);
//	}
//
//	SDL_GL_DeleteContext(context);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//
//
//
//	return 0;
//}