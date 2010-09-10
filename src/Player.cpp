#include <string>
#include <SDL/SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"
#include "globals.h"

using namespace std;

Player::Player(SDL_Surface* image, int _x, int _y, double _move_spd, double _jump_spd, double _ay) : Object(image, _x, _y) {
	midjump = false;
	dx = 0;
	dy = 0;
	ax = 0;
	ay = _ay;
	move_spd = _move_spd;
	jump_spd = _jump_spd;
}

Player::~Player() {
	// Nothing player-specific to destroy
}

// Update player state
void Player::update() {
	updateSprite();

	// If player is not moving
	if (dx == 0 && dy == 0 && midjump == false) {
		startSprite(0);
	}

	// Set velocities according to user input
	Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_UP] && dy == 0 && midjump == false) {
		// Jump up
		// We need to check that player is not falling or in middle of a jump before
		// setting jump velocity
		midjump = true;
		dy = jump_spd;
		// Disable x acceleration on start of jump because you don't want any
		// remaining horizontal force unless left or right keys are pressed.
		ax = 0;
		// Initiate jump animation
		startSprite(2);
	}
	else if (keystate[SDL_SCANCODE_LEFT]) {
		// Move left
		// Cancel out previous horizontal velocity
		if (ax > 0)
			dx = 0;
		ax = -g_axfactor * move_spd;
		// Initiate walking animation if not midjump
		if (midjump == false)
			startSprite(1);
	}
	else if (keystate[SDL_SCANCODE_RIGHT]) {
		// Move right
		// Cancel out previous horizontal velocity
		if (ax < 0)
			dx = 0;
		ax = g_axfactor * move_spd;
		// Initiate walking animation if not midjump
		if (midjump == false)
			startSprite(1);
	}
	else {
		ax = 0;
		if (midjump == false) {
			// If we're not midjump and player is not moving, we should simulate friction
			// For now just set x velocity to zero
			dx = 0;
		}
	}

	// Apply x velocity and acceleration
	x += dx;
	dx += ax;
	// Cap x velocity at move_spd
	if (dx > move_spd) {
		dx = move_spd;
	}
	else if (dx < -move_spd) {
		dx = -move_spd;
	}
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

	// Apply y velocity and acceleration
	y += dy;
	dy += ay;
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
}
