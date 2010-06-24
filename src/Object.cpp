#include <SDL/SDL.h>

#include "Object.h"
#include "GameWindow.h"
#include "helpers.h"

Object::Object(int _x, int _y, string imageFile) {
	x = _x;
	y = _y;
	surface = IMG_Load(imageFile.c_str());
	window = NULL;
}

Object::~Object() {
	SDL_FreeSurface(surface);
}

void Object::setWindow(GameWindow* win) {
	window = win;
	// Now that we know the window we should reformat the object surface
	// to match the window's surface for faster blitting.
	SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
	SDL_FreeSurface(surface);
	surface = temp;
}

// Check for collision between two objects
bool Object::checkCollision(Object& obj) {

}

// Update function that updates object state
void Object::update() {
	// Because this is a generic object, do nothing.
}

// Draw function that blits object surface to the window
void Object::draw() {
	SDL_Rect drawRect = SDL_CreateRect(x, y);
	SDL_BlitSurface(surface, NULL, window->area(), &drawRect);
}
