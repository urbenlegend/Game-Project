#pragma once

#include <string>
#include <vector>
#include <SDL.h>

using namespace std;

class GameWindow;

/* Object Class: Generic object class that all game objects have to inherit from.
   Supports bounding box collisions and sprite animations.
*/
class Object {
protected:
	// Surface containing the object's appearance
	SDL_Surface* surface;
	// Window this object is registered to
	GameWindow* window;

public:
	// Position variables
	int x;
	int y;

	Object(SDL_Surface* image, int _x, int _y);
	virtual ~Object();

	virtual void setWindow(GameWindow* win);
	virtual void setSurface(SDL_Surface* image);

	virtual bool checkCollision(Object& obj) const;

	virtual void update();
	virtual void draw();

	virtual int width() const;
	virtual int height() const;
};
