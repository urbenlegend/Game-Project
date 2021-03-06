#include <vector>
#include <fstream>
#include "SDL_headers.h"

#include "GameWindow.h"
#include "Object.h"
#include "Player.h"
#include "util.h"
#include "globals.h"



using namespace std;

struct player_t {
	string sprite;
	double move_spd;
};

GameWindow::GameWindow(int w, int h) {
	cam_x = 0;
	cam_y = 0;
	// Initialize game window
	window = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE|SDL_ANYFORMAT);
	// Create a surface for game objects to paint on and add to asset manager.
	textures.add("level_surface", SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
	// Get surface pointer from asset manager
	surface = textures["level_surface"];
}

GameWindow::~GameWindow() {
	// Delete level objects
	for (size_t y = 0; y < level.size(); y++) {
		for (size_t x = 0; x < level[y].size(); x++) {
			delete level[y][x];
		}
	}

	// Update projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		delete projectiles[i];
	}

	// Delete players
	for (size_t i = 0; i < players.size(); i++) {
		delete players[i];
	}
	// Deleting obstacles should be deleted from level objects since
	// we delete the actual object and the obstacle data structure only has
	// pointers right? Or do we have to set those pointers to null?

}

SDL_Surface* GameWindow::getSurface() {
	return surface.get();
}

int GameWindow::width() {
	return surface->w;
}

int GameWindow::height() {
	return surface->h;
}

// Tells game window to keep track of a level object
void GameWindow::addObject(Object* obj) {
	obj->setWindow(this);
	objects.push_back(obj);
}

// Tells game window to keep track of a projectile
void GameWindow::addProjectile(Object* prj) {
	prj->setWindow(this);
	projectiles.push_back(prj);
}

// Tells game window to keep track of a collidable object
void GameWindow::addObstacle(Object* obj) {
	obstacles.push_back(obj);
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
    vector<player_t> plrs;
	string tile_data;
	int tile_width;
	int tile_height;
    string line;

    ifstream infile(filename.c_str());
    if (!infile.is_open()) {
    	// Return failure if filename is invalid
    	return -1;
    }

	while (getline(infile, line)) {
		// Skip commented lines
		if (line[0] == '#') continue;

		vector<string> line_tokens;
		tokenize(line, line_tokens);
		if (line_tokens.size() >= 3 && line_tokens[0] == "player") {
			player_t plr;
			plr.sprite = line_tokens[1];
			plr.move_spd = atof(line_tokens[2].c_str());
			plrs.push_back(plr);
		}
		else if (line_tokens.size() >= 2 && line_tokens[0] == "tile_set") {
			textures.add("level_tileset", IMG_OptimizedLoadAlpha(line_tokens[1].c_str()));
			tile_data = line_tokens[2];
		}
		else if (line_tokens.size() >= 2 && line_tokens[0] == "tile_width") {
			tile_width = atoi(line_tokens[1].c_str());
		}
		else if (line_tokens.size() >= 2 && line_tokens[0] == "tile_height") {
			tile_height = atoi(line_tokens[1].c_str());
		}
		else if (line_tokens.size() >= 1 && line_tokens[0] == "mapstart") {
			size_t row = 0;
			while (getline(infile,line)) {
				// Skip commented lines
				if (line[0] == '#') continue;
				tokenize(line, line_tokens);
				if (line_tokens.size() >= 1 && line_tokens[0] != "mapend") {
					// Create new row of tiles
					level.push_back(vector<Object*>());
					for (size_t col = 0; col < line.length(); col++) {
						// If symbol is a player spawn number, load corresponding player from player_pics
						if (isdigit(line[col])) {
							// Convert char to an integer
							char symbol[2];
							symbol[0] = line[col];
							symbol[1] = 0;
							size_t num = atoi(symbol);
							// Load player
							if (num < plrs.size()) {
								Player* new_player = new Player(0, 0, plrs[num].move_spd);
								load_status = new_player->loadSprite(plrs[num].sprite);
								new_player->x = width()/2 + tile_width/2 * col - tile_width/2 * row;
								new_player->y = tile_height/2 * col + tile_height/2 * row;
								addPlayer(new_player);
							}
							else {
								load_status = 1;
							}
						}
						else if (isalpha(line[col])) {
							// Create a default object with no surface.
							Object* tile = new Object(0, 0);
							// Load tile_set as sprite data. Start sprite for the tile indicated by the map index so that we can get size data
							// Currently assuming that the map index starts at char 'A'. Can change this later.
							tile->setWindow(this);
							tile->setSurface(textures["level_tileset"]);
							tile->loadSprite(tile_data.c_str());
							tile->startSprite(line[col] - 'A');
							tile->x = width()/2 + tile_width/2 * col - tile_width/2 * row;
							tile->y = tile_height/2 * col + tile_height/2 * row;
							level.back().push_back(tile);
						}
					}
					row++;
				}
			}
		}
	}
	return load_status;
}

// Update states of all objects that its keeping track of
void GameWindow::update() {
	// Tell all level tiles to update
	for (size_t y = 0; y < level.size(); y++) {
		for (size_t x = 0; x < level[y].size(); x++) {
			level[y][x]->update();
		}
	}
	// Tell general objects to update their state
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}
	// Update projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->update();
	}
	// Update players
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->update();
	}

	// Adjust camera
	cam_x = players[0]->x - window->w/2;
	cam_y = players[0]->y - window->h/2;
}

// Draw window
void GameWindow::draw() {
	// Blank screen
	SDL_FillRect(window, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
	SDL_FillRect(surface.get(), NULL, SDL_MapRGB(surface->format, 0, 0, 0));

	// Draw all level tiles
	for (size_t y = 0; y < level.size(); y++) {
		for (size_t x = 0; x < level[y].size(); x++) {
			level[y][x]->draw();
		}
	}
	// Draw general objects
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}
	// Draw projectiles
	for (size_t i = 0; i < projectiles.size(); i++) {
		projectiles[i]->draw();
	}
	// Draw players
	for (size_t i = 0; i < players.size(); i++) {
		players[i]->draw();
	}

	// Paint surface onto window according to cam pos
	SDL_Rect cam_rect;
	cam_rect.x = -cam_x;
	cam_rect.y = -cam_y;
	SDL_BlitSurface(surface.get(), NULL, window, &cam_rect);

	// Flip surface after all objects have drawn
	SDL_Flip(window);
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
					window = SDL_SetVideoMode(window->w, window->h, 32, (window->flags & SDL_FULLSCREEN ? window->flags & ~SDL_FULLSCREEN: window->flags | SDL_FULLSCREEN));
				}
			}
		}

		// Cap fps so game doesn't draw too fast and use up all the cpu
		cap_fps(start, cg_fps);
	}
}
