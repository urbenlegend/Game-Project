#include <vector>
#include <SDL/SDL.h>

#include "GameWindow.h"
#include "Object.h"

using namespace std;

GameWindow::GameWindow(int w, int h) {
	surface = SDL_SetVideoMode(w, h, 32, SDL_ANYFORMAT);
}

GameWindow::~GameWindow() {
	for (size_t i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
}

// Tells game window to keep track of a level object
void GameWindow::addLevelObj(Object* obj) {
	obj->setWindow(this);
	level.push_back(obj);
}

// Tells game window to keep track of a projectile
void GameWindow::addProjectile(Object* obj) {
	obj->setWindow(this);
	projectiles.push_back(obj);
}

// Add player to window
void GameWindow::addPlayer(Object* obj) {
	obj->setWindow(this);
	players.push_back(obj);
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

	// Draw Level
	for (size_t i = 0; i < level.size(); i++) {
		level[i]->draw();
	}

	// Draw projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
	}

	// Draw players
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}

	// Flip surface after all objects have drawn
	SDL_Flip(surface);
}
