#pragma once

#include <vector>
#include <string>
#include "SDL_headers.h"

#include "Assets.h"

using namespace std;

class Object;
class Player;

/* GameWindow Class: Container for all game objects. Its main game loop
 * ensures that all objects are updating their states and painting properly
 */
class GameWindow {
private:
	// Camera pos
	int cam_x;
	int cam_y;
	// Window surface
	SDL_Surface* window;
	// Level surface
	shared_ptr<SDL_Surface> surface;
	// Tileset surface
	SDL_Surface* tileset;
	// Assets
	AssetManager<SDL_Surface> textures;
	
public:
	// Level tile objects
	vector<vector<Object*>> level;
	// General objects
	vector<Object*> objects;
	// Projectiles that in the game window
	vector<Object*> projectiles;
	// Players
	vector<Player*> players;
	// Collidable Objects
	vector<Object *> obstacles;

	GameWindow(int w, int h);
	~GameWindow();

	SDL_Surface* getSurface();
	int width();
	int height();

	void addObject(Object* obj);
	void addProjectile(Object* prj);
	void addObstacle(Object * collider);
	void addPlayer(Player* plyr);
	int loadLevel(string filename);

	void update();
	void draw();

	void play();
};
