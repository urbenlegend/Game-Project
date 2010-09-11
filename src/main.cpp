#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "GameWindow.h"
#include "Player.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Initialize SDL video
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << SDL_GetError();
		return -1;
	}

	// Start the game
	GameWindow window(1280, 800);

	// Load level
	int success = window.loadLevel("data/testlevel.txt");

    // Begin game loop
    if (success != -1)
		window.play();

	SDL_Quit();

	return success;
}
