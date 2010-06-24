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

	Object(int _x, int _y, string imageFile);
	virtual ~Object();
	virtual void setWindow(GameWindow* win);

	virtual bool checkCollision(Object& obj);

	virtual void update();
	virtual void draw();
};

#endif // OBJECT_H
