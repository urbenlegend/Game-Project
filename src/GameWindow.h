#pragma once

#include <vector>
#include <string>
#include "SDL_headers.h"

using namespace std;

class Object;
class Player;

/* GameWindow Class: Container for all game objects. Its main game loop
 * ensures that all objects are updating their states and painting properly
 */
class GameWindow {
private:
	// Window surface
	SDL_Surface* surface;
	// Tileset surface
	SDL_Surface* tileset;
	// Level tile objects
	vector<vector<Object*>> level;
	// General objects
	vector<Object*> objects;
	// Projectiles that in the game window
	vector<Object*> projectiles;
	// Players
	vector<Player*> players;

public:
	GameWindow(int w, int h);
	~GameWindow();

	SDL_Surface* getSurface();
	int width();
	int height();
	vector<vector<Object*>>* getLevel();
	vector<Object*>* getObjects();
	vector<Object*>* getProjectiles();
	vector<Player*>* getPlayers();

	void addObject(Object* obj);
	void addProjectile(Object* prj);
	void addPlayer(Player* plyr);
	int loadLevel(string filename);

	void update();
	void draw();

	void play();
};
