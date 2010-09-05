#include <vector>
#include <fstream>
#include <SDL/SDL.h>

#include "GameWindow.h"
#include "Object.h"
#include "Player.h"
#include "helpers.h"

using namespace std;

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

void GameWindow::loadLevel(string filename)
{
    int tile_height;
    int tile_width;
    string line;

    ifstream infile(filename.c_str());
    if (infile.is_open())
    {
        while (!infile.eof())
        {
            infile >> line;
            if (line == "tilewidth")
                infile >> tile_width;
            else if (line == "tileheight")
                infile >> tile_height;
            else if (line == "mapstart")
            {
                int row = 0;
                int col = 0;
                while (line != "mapend")
                {
                    getline(infile,line);
                    for (col = 0; col < line.length(); col++)
                    {
                        if (line[col] == '-')
                        {
                            SDL_Surface* block = SDL_CreateRGBSurface(SDL_HWSURFACE, tile_width, tile_height, 32, 0, 0, 0, 0);
                            SDL_FillRect(block, NULL, SDL_MapRGB(block->format, 255, 255, 255));
                            addLevelObj(new Object(block, col*tile_width, row*tile_height));
                        }
                        else if (line[col] == '*')
                        {
                            addPlayer(new Player(IMG_Load("data/player1.jpg"), col*tile_width, row*tile_height, 10, -20, 1));
                        }
                    }
                    row++;
                }
            }
        }
    }
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
		cap_fps(start, 60);
	}
}
