#include <string>
#include <SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"
#include "globals.h"

using namespace std;

Player::Player(int _x, int _y, double _move_spd, double _jump_spd, double _ay) : Object(_x, _y) {
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

	// Set velocities according to user input
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_UP] && dy == 0 && midjump == false) {
		y -= 3;
		// Initiate walking animation
		startSprite(1);
	}
	else if (keystate[SDLK_DOWN]) {
		y += 3;
		// Initiate walking animation
		startSprite(1);
	}
	if (keystate[SDLK_LEFT]) {
		x -= 3;
		// Initiate walking animation
		startSprite(1);
	}
	else if (keystate[SDLK_RIGHT]) {
		x += 3;
		// Initiate walking animation
		startSprite(1);
	}
	if (!keystate[SDLK_UP] && !keystate[SDLK_DOWN] && !keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]) {
		// Otherwise do the standing animation
		startSprite(0);
	}
}
