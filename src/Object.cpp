#include <SDL/SDL.h>

#include "Object.h"
#include "GameWindow.h"
#include "helpers.h"

Object::Object(SDL_Surface* image, int _x, int _y) {
	x = _x;
	y = _y;
	surface = image;
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

void Object::setSurface(SDL_Surface* image) {
	// Delete previous surface and set image as surface
	SDL_FreeSurface(surface);
	surface = image;

	// Convert surface to window format if window is known
	if (window != NULL) {
		SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
		SDL_FreeSurface(surface);
		surface = temp;
	}
}


// Check for collision between two objects using basic bounding boxes
bool Object::checkCollision(Object& obj) {
	// Bounding extents for this object
	int left1 = x;
	int right1 = x + surface->w;
	int top1 = y;
	int bottom1 = y + surface->h;
	// Bounding extents for obj
	int left2 = obj.x;
	int right2 = obj.x + obj.surface->w;
	int top2 = obj.y;
	int bottom2 = obj.y + obj.surface->h;

	if (left1 > right2) return false;
	if (right1 < left2) return false;
	if (top1 > bottom2) return false;
	if (bottom1 < top2) return false;

	return true;
}

// Update function that updates object state
void Object::update() {
	// Because this is a generic object, do nothing.
}

// Draw function that blits object surface to the window
void Object::draw() {
	SDL_Rect drawRect = SDL_CreateRect(x, y);
	SDL_BlitSurface(surface, NULL, window->getSurface(), &drawRect);
}
