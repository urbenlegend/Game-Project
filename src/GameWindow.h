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
	// Objects that are part of the level
	vector<Object*> level;
	// Projectiles that in the game window
	vector<Object*> projectiles;
	// Players
	vector<Object*> players;

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

	void addLevelObj(Object* obj);
	void addProjectile(Object* prj);
	void addPlayer(Object* plyr);

	void update();
	void draw();

	void play();
};

#endif // GAMEWINDOW_H
