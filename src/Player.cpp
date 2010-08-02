#include <string>
#include <SDL/SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"

using namespace std;

Player::Player(string imageFile, int _x, int _y, int _move_spd, int _jump_spd, int _weight) : Object(imageFile, _x, _y) {
	dx = 0;
	dy = 0;
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

	// Apply velocities
	x += dx;
	y += dy;
	// Apply accelerations
	dy += weight;

	// Collision detection with walls of window
	// TODO: collision detection with other objects
	if (x >= window->width() - surface->w) {
		x = window->width() - surface->w;
	}
	else if (x < 0) {
		x = 0;
	}
	if (y >= window->height() - surface->h) {
		y = window->height() - surface->h;
		dy = 0;
		midjump = false;
	}
	else if (y < 0) {
		y = 0;
		dy = 0;
	}

	// If we're not midjump we should simulate friction
	// For now just set x velocity to zero
	if (midjump == false) {
		dx = 0;
	}
}
