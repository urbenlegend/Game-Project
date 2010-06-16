#include <SDL/SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"
#include "helpers.h"

using namespace std;

Player::Player(int _x, int _y, int _rot, int move, int jump, int wgt) : Object(_x, _y, _rot){
	surface = NULL;
	dx = 0;
	dy = 0;
	midjump = false;
	move_speed = move;
	jump_speed = jump;
	weight = wgt;
}

Player::~Player() {
	//dtor
}

void Player::setWindow(GameWindow* win) {
	window = win;
	// Initialize player image with pixel format of window
	// It's just a white rectangle for now
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 70, window->area()->format->BitsPerPixel,
											 0x00ff0000, 0x0000ff00, 0x000000ff, (window->area()->format->BitsPerPixel == 32 ? 0xff000000 : 0));
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
}

// Update player state
void Player::update() {
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_LEFT])
		x -= move_speed;
	else if (keystate[SDLK_RIGHT])
		x += move_speed;
	if (keystate[SDLK_UP] && midjump == false) {
		midjump = true;
		dy = jump_speed;
	}

	// Apply velocities
	y += dy;
	// Apply accelerations
	dy += weight;

	// Collision detection with walls of window
	// TODO: collision detection with other objects
	if (x > window->width() - surface->w) {
		x = window->width() - surface->w;
	}
	else if (x < 0) {
		x = 0;
	}
	if (y > window->height() - surface->h) {
		y = window->height() - surface->h;
		dy = 0;
		midjump = false;
	}
	else if (y < 0) {
		y = 0;
		dy = 0;
	}

}

// Draw player to window surface
void Player::draw() {
	SDL_Rect drawRect = SDL_CreateRect(x, y);
	SDL_BlitSurface(surface, NULL, window->area(), &drawRect);
}
