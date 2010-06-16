#include <vector>
#include <SDL/SDL.h>

#include "GameWindow.h"
#include "Object.h"

using namespace std;

GameWindow::GameWindow(int w, int h) {
	surface = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE|SDL_ANYFORMAT);
}

GameWindow::~GameWindow() {
	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}

// Functions to get information about game surface
SDL_Surface* GameWindow::getSurface() {
	return surface;
}
int GameWindow::getWidth() {
	return surface->w;
}
int GameWindow::getHeight() {
	return surface->h;
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
