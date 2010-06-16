#include <iostream>
#include <SDL/SDL.h>

#include "helpers.h"
#include "GameWindow.h"
#include "Player.h"

using namespace std;

void startGame() {
	// Initialize game data
	GameWindow window(1280, 800);
	Player player(10, 10, 0);
	window.trackObject(&player);

	// Run main game loop
	while (true) {
		Uint32 start = SDL_GetTicks();

		window.update();
		window.draw();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return;
				if (event.key.keysym.sym == SDLK_f) {
					SDL_Surface* surface = window.getSurface();
					surface = SDL_SetVideoMode(surface->w, surface->h, 32, (surface->flags & SDL_FULLSCREEN ? 0 : SDL_FULLSCREEN));
				}
			}
		}

		// Cap fps so game doesn't draw too fast and use up all the cpu
		cap_fps(start, 30);
	}
}

int main(int argc, char* argv[]) {
	// Initialize SDL video
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << SDL_GetError();
		return -1;
	}

	// Start the game
	startGame();

	SDL_Quit();

	return 0;
}
