#include <SDL/SDL.h>

#include "Object.h"
#include "GameWindow.h"
#include "helpers.h"

Object::Object(SDL_Surface* image, int _x, int _y) {
	x = _x;
	y = _y;
	dx = 0;
	dy = 0;
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
// new_x and new_y are optional reposition coordinates for the calling object.
// They help in repositioning an object after a collision is detected.
// If new_x and new_y are specified, object velocity is taken into consideration.
inline bool Object::checkCollision(Object& obj) const {
	// Bounding extents for this object
	int left1 = x;
	int right1 = x + width() - 1;
	int top1 = y;
	int bottom1 = y + height() - 1;
	// Bounding extents for obj
	int left2 = obj.x;
	int right2 = obj.x + obj.width() - 1;
	int top2 = obj.y;
	int bottom2 = obj.y + obj.height() - 1;

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
void Object::draw() const {
	SDL_Rect drawRect = SDL_CreateRect(x, y);
	SDL_BlitSurface(surface, NULL, window->getSurface(), &drawRect);
}
