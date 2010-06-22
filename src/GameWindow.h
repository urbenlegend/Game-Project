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

	// Inline functions to get information about game surface
	SDL_Surface* area() {
		return surface;
	}
	int width() {
		return surface->w;
	}
	int height() {
		return surface->h;
	}

	void trackObject(Object* obj);

	void update();
	void draw();

	void play();
};

#endif // GAMEWINDOW_H
