#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <vector>
#include <string>
#include <SDL/SDL.h>

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
	const vector<Object*>& getLevel();
	const vector<Object*>& getProjectiles();
	const vector<Player*>& getPlayers();

	void addLevelObj(Object* obj);
	void addProjectile(Object* prj);
	void addPlayer(Player* plyr);
	int loadLevel(string filename);

	void update();
	void draw();

	void play();
};

// Functions to get information about game surface
inline SDL_Surface* GameWindow::getSurface() {
	return surface;
}
inline int GameWindow::width() {
	return surface->w;
}
inline int GameWindow::height() {
	return surface->h;
}
inline const vector<Object*>& GameWindow::getLevel() {
	return level;
}
inline const vector<Object*>& GameWindow::getProjectiles() {
	return projectiles;
}
inline const vector<Player*>& GameWindow::getPlayers() {
	return players;
}

#endif // GAMEWINDOW_H
