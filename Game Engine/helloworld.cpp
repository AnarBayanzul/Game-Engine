//#include <SDL.h>
//
//int main(int argc, char** argv) {
//	SDL_Init(SDL_INIT_VIDEO);
//
//	SDL_Window* window = SDL_CreateWindow("Testing SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
//	SDL_Surface* buffer = SDL_GetWindowSurface(window);
//	SDL_FillRect(buffer, NULL, SDL_MapRGB(buffer->format, 0xFF, 0x00, 0xFF));
//
//	bool exit = false;
//	SDL_Event e;
//	while (!exit) {
//		// Game loop!
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
//		SDL_UpdateWindowSurface(window);
//	}
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//
//	return 0;
//
//}