#include <string>
#include <SDL/SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"

using namespace std;

Player::Player(SDL_Surface* image, int _x, int _y, int _move_spd, int _jump_spd, int _weight) : Object(image, _x, _y) {
	midjump = false;
	move_spd = _move_spd;
	jump_spd = _jump_spd;
	weight = _weight;
}

Player::~Player() {
	// Nothing player-specific to destroy
}

// Update player state
void Player::update() {
	// Set velocities according to user input
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_LEFT]) {
		// Move left
		dx = -move_spd;
	}
	else if (keystate[SDLK_RIGHT]) {
		// Move right
		dx = move_spd;
	}
	if (keystate[SDLK_UP] && dy == 0 && midjump == false) {
		// Jump up
		// We need to check that player is not falling or in middle of a jump before
		// setting jump velocity
		midjump = true;
		dy = jump_spd;
	}

	// Apply x velocity
	x += dx;
	// Collision detection with level objects in the x axis
	const vector<Object*>& level = window->getLevel();
	// Compare extents with all collidable level objects
	for (size_t i = 0; i < level.size(); i++) {
		if (checkCollision(*level[i])) {
			if (dx < 0) {
                x = level[i]->x + level[i]->width();
            }
            else if (dx > 0) {
                x = level[i]->x - width();
            }
			dx = 0;
		}
	}

	// Apply y velocity and gravity acceleration
	y += dy;
	dy += weight;
	// Collision detection with level objects in the y axis
	for (size_t i = 0; i < level.size(); i++) {
		if (checkCollision(*level[i])) {
            if (dy < 0) {
                y = level[i]->y + level[i]->height();
            }
            else if (dy > 0) {
                y = level[i]->y - height();
                midjump = false;
            }
			dy = 0;
		}
	}

	// If we're not midjump we should simulate friction
	// For now just set x velocity to zero
	if (midjump == false) {
		dx = 0;
	}
}
