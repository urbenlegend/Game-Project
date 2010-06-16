#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <SDL/SDL.h>

using namespace std;

class Object;

class GameWindow {
private:
	// Window surface
	SDL_Surface* surface;
	// Objects that this game window is keepign track of
	vector<Object*> objects;

public:
	GameWindow(int w, int h);
	~GameWindow();

	SDL_Surface* getSurface();
	int getWidth();
	int getHeight();

	void trackObject(Object* obj);

	void update();
	void draw();
};

#endif // GAMEWINDOW_H
