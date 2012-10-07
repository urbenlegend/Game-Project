#pragma once

#include <vector>
#include <string>
#include <SDL.h>

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
	// Objects that are part of the level
	vector<Object*> level;
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
	vector<Object*>* getLevel();
	vector<Object*>* getProjectiles();
	vector<Player*>* getPlayers();

	void addLevelObj(Object* obj);
	void addProjectile(Object* prj);
	void addPlayer(Player* plyr);
	int loadLevel(string filename);

	void update();
	void draw();

	void play();
};

// Functions to get information about game surface

