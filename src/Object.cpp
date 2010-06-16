#include <SDL/SDL.h>

#include "Object.h"
#include "GameWindow.h"

Object::Object(int _x, int _y, int _rot) {
	x = _x;
	y = _y;
	rot = _rot;
	window = NULL;
}

void Object::setWindow(GameWindow* win) {
	window = win;
}
