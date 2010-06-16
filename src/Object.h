#ifndef OBJECT_H
#define OBJECT_H

#include <SDL/SDL.h>

using namespace std;

class GameWindow;

class Object {
protected:
	// Window this object is registered to
	GameWindow* window;

public:
	// Position variables
	int x;
	int y;
	int rot;

	Object(int _x, int _y, int _rot);
	virtual ~Object() {}
	void setWindow(GameWindow* win);

	virtual void update() = 0;
	virtual void draw() = 0;
};

#endif // OBJECT_H
