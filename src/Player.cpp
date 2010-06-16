#include <SDL/SDL.h>

#include "Player.h"
#include "Object.h"
#include "GameWindow.h"

using namespace std;

Player::Player(int _x, int _y, int _rot) : Object(_x, _y, _rot){

}

Player::~Player() {
	//dtor
}

// Update player state
void Player::update() {
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_LEFT]) x -= 3;
	else if (keystate[SDLK_RIGHT]) x += 3;
	if (keystate[SDLK_UP]) y -= 3;
	else if (keystate[SDLK_DOWN]) y += 3;
}

// Draw player to window surface
void Player::draw() {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = 10;
	rect.h = 10;
	SDL_FillRect(window->getSurface(), &rect, SDL_MapRGB(window->getSurface()->format, 255, 255, 255));
}
