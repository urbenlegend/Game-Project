#include <vector>
#include <SDL/SDL.h>

#include "GameWindow.h"
#include "Object.h"
#include "helpers.h"

using namespace std;

GameWindow::GameWindow(int w, int h) {
	surface = SDL_SetVideoMode(w, h, 32, SDL_ANYFORMAT);
}

GameWindow::~GameWindow() {
	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}

// Tells game window to keep track of object
void GameWindow::trackObject(Object* obj) {
	obj->setWindow(this);
	objects.push_back(obj);
}

// Update states of all objects that its keeping track of
void GameWindow::update() {
	// Tell all objects to update their state
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}
}

// Draw window
void GameWindow::draw() {
	// Blank screen
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

	// Tell all objects to draw to surface
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}

	// Flip surface after all objects have drawn
	SDL_Flip(surface);
}

// Main game loop
void GameWindow::play() {
	// Run main game loop
	while (true) {
		Uint32 start = SDL_GetTicks();

		update();
		draw();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return;
				if (event.key.keysym.sym == SDLK_f) {
					surface = SDL_SetVideoMode(surface->w, surface->h, 32, (surface->flags & SDL_FULLSCREEN ? surface->flags & ~SDL_FULLSCREEN: surface->flags | SDL_FULLSCREEN));
				}
			}
		}

		// Cap fps so game doesn't draw too fast and use up all the cpu
		cap_fps(start, 30);
	}
}
