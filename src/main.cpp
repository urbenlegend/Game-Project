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

	window.addPlayer(new Player(IMG_Load("data/player1.jpg"), 10, 10, 20, -30, 2));
	SDL_Surface* block1 = SDL_CreateRGBSurface(SDL_HWSURFACE, 400, 30, 32, 0, 0, 0, 0);
	SDL_FillRect(block1, NULL, SDL_MapRGB(block1->format, 255, 255, 255));
	window.addLevelObj(new Object(block1, 600, 700));
	SDL_Surface* block2 = SDL_CreateRGBSurface(SDL_HWSURFACE, 400, 30, 32, 0, 0, 0, 0);
	SDL_FillRect(block2, NULL, SDL_MapRGB(block2->format, 255, 255, 255));
	window.addLevelObj(new Object(block2, 800, 300));
	SDL_Surface* block3 = SDL_CreateRGBSurface(SDL_HWSURFACE, 400, 30, 32, 0, 0, 0, 0);
	SDL_FillRect(block3, NULL, SDL_MapRGB(block3->format, 255, 255, 255));
	window.addLevelObj(new Object(block3, 200, 500));

	window.play();

	SDL_Quit();

	return 0;
}
