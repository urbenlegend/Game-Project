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
bool Object::checkCollision(Object& obj, int* new_x, int* new_y) const {
	// Object coordinates
	int x1 = x;
	int y1 = y;
	int x2 = obj.x;
	int y2 = obj.y;
	// If reposition variables are specified, factor in object velocity
	if (new_x != NULL | new_y != NULL) {
		x1 += dx;
		y1 += dy;
		x2 += obj.dx;
		y2 += obj.dy;
	}

	// Bounding extents for this object
	int left1 = x1;
	int right1 = x1 + width() - 1;
	int top1 = y1;
	int bottom1 = y1 + height() - 1;
	// Bounding extents for obj
	int left2 = x2;
	int right2 = x2 + obj.width() - 1;
	int top2 = y2;
	int bottom2 = y2 + obj.height() - 1;

	if (left1 > right2) return false;
	if (right1 < left2) return false;
	if (top1 > bottom2) return false;
	if (bottom1 < top2) return false;

	// Calculate non-colliding position by scaling back along dx-dy vector
	if (new_x != NULL | new_y != NULL) {
		int scalarx = 1;
		int scalary = 1;
		if (dx > 0) {
			if (right1 >= left2) {
				scalarx = (left2 - right1) / dx;
			}
		}
		else if (dx < 0) {
			if (left1 <= right2) {
				scalarx = (right2 - left1) / dx;
			}
		}
		if (dy > 0) {
			if (bottom1 >= top2) {
				scalary = (top2 - bottom1) / dy;
			}
		}
		else if (dy < 0) {
			if (top1 <= bottom2) {
				scalary = (bottom2 - top1) / dy;
			}
		}
		// Use scalar value that scales back the least
		int scalar = scalarx > scalary ? scalarx : scalary;
		// Set reposition variables
		if (new_x != NULL) {
			*new_x = x + scalar * dx;
		}
		if (new_y != NULL) {
			*new_y = y + scalar * dy - 1;
		}
	}

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
