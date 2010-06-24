#include <iostream>
#include <SDL/SDL.h>

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
	window.addPlayer(new Player(10, 10, 20, -30, 2, "data/player1.jpg"));
	window.play();

	SDL_Quit();

	return 0;
}
