#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <SDL/SDL.h>

using namespace std;

class GameWindow;

/* Object Class: Generic object class that all game objects have to inherit from */
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

	int width();
	int height();

	virtual bool checkCollision(Object& obj);

	virtual void update();
	virtual void draw();
};

// Functions to access surface attributes
inline int Object::width() {
	return surface->w;
}
inline int Object::height() {
	return surface->h;
}
#endif // OBJECT_H
