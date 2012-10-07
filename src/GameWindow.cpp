#include <vector>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>

#include "GameWindow.h"
#include "Object.h"
#include "Player.h"
#include "globals.h"

using namespace std;

struct player_t {
	string pic;
	string sprite;
	double move_spd;
	double jump_spd;
	double weight;
};

GameWindow::GameWindow(int w, int h) {
	surface = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE|SDL_ANYFORMAT);
}

GameWindow::~GameWindow() {
	// Delete level objects
	for (size_t i = 0; i < level.size(); i++) {
		delete level[i];
	}

	// Update projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		delete projectiles[i];
	}

	// Delete players
	for (size_t i = 0; i < players.size(); i++) {
		delete players[i];
	}
}

SDL_Surface* GameWindow::getSurface() {
	return surface;
}
int GameWindow::width() {
	return surface->w;
}
int GameWindow::height() {
	return surface->h;
}
const vector<Object*>& GameWindow::getLevel() {
	return level;
}
const vector<Object*>& GameWindow::getProjectiles() {
	return projectiles;
}
const vector<Player*>& GameWindow::getPlayers() {
	return players;
}

// Tells game window to keep track of a level object
void GameWindow::addLevelObj(Object* obj) {
	obj->setWindow(this);
	level.push_back(obj);
}

// Tells game window to keep track of a projectile
void GameWindow::addProjectile(Object* prj) {
	prj->setWindow(this);
	projectiles.push_back(prj);
}

// Add player to window
void GameWindow::addPlayer(Player* plyr) {
	plyr->setWindow(this);
	players.push_back(plyr);
}

// Loads a tile-based level from textfile specified by filename
// Returns 0 if level is loaded successfully
// -1 if level completely fails to load
// > 0 if level is partially loaded.
int GameWindow::loadLevel(string filename) {
	int load_status = 0;
    int tile_height;
    int tile_width;
    vector<player_t> plrs;
    string line;

    ifstream infile(filename.c_str());
    if (!infile.is_open()) {
    	// Return failure if filename is invalid
    	return -1;
    }

	while (!infile.eof()) {
		infile >> line;
		if (line == "tilewidth")
			infile >> tile_width;
		else if (line == "tileheight")
			infile >> tile_height;
		else if (line == "player") {
			player_t plr;
			infile >> plr.sprite;
			infile >> plr.move_spd;
			infile >> plr.jump_spd;
			infile >> plr.weight;
			plrs.push_back(plr);
		}
		else if (line == "mapstart") {
			size_t row = 0;
			size_t col = 0;
			while (line != "mapend") {
				getline(infile,line);
				for (col = 0; col < line.length(); col++) {
					if (line[col] == '-') {
						SDL_Surface* block = SDL_CreateRGBSurface(SDL_HWSURFACE, tile_width, tile_height, 32, 0, 0, 0, 0);
						SDL_FillRect(block, NULL, SDL_MapRGB(block->format, 255, 255, 255));
						addLevelObj(new Object(block, col*tile_width, row*tile_height));
					}
					// If symbol is a player spawn number, load corresponding player from player_pics
					else if (isdigit(line[col])) {
						// Convert char to to integer
						char symbol[2];
						symbol[0] = line[col];
						symbol[1] = 0;
						size_t num = atoi(symbol);
						// Load player
						if (num < plrs.size()) {
							Player* new_player = new Player(NULL, col*tile_width, row*tile_height, plrs[num].move_spd, plrs[num].jump_spd, plrs[num].weight);
							load_status = new_player->loadSprite(plrs[num].sprite);
							addPlayer(new_player);
						}
						else {
							load_status = 1;
						}
					}
				}
				row++;
			}
		}
	}
	return load_status;
}

// Update states of all objects that its keeping track of
void GameWindow::update() {
	// Tell level objects to update their state
	for (size_t i = 0; i < level.size(); i++) {
		level[i]->update();
	}
	// Update projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->update();
	}
	// Update players
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->update();
	}
}

// Draw window
void GameWindow::draw() {
	// Blank screen
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

	// Draw Level
	for (size_t i = 0; i < level.size(); i++) {
		level[i]->draw();
	}
	// Draw projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
	}
	// Draw players
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}

	// Flip surface after all objects have drawn
	SDL_Flip(surface);
}

// Main game loop
void GameWindow::play() {
	// Run main game loop
	while (true) {
		Uint32 start = SDL_GetTicks();

		update();
		draw();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return;
				if (event.key.keysym.sym == SDLK_f) {
					surface = SDL_SetVideoMode(surface->w, surface->h, 32, (surface->flags & SDL_FULLSCREEN ? surface->flags & ~SDL_FULLSCREEN: surface->flags | SDL_FULLSCREEN));
				}
			}
		}

		// Cap fps so game doesn't draw too fast and use up all the cpu
		cap_fps(start, cg_fps);
	}
}
