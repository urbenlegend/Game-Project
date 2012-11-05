#include <string>
#include "SDL_headers.h"
#include "Player.h"
#include "Object.h"
#include "GameWindow.h"
#include "util.h"

using namespace std;

Player::Player(int _x, int _y, double _move_spd) : Object(_x, _y) {
	midjump = false;
	dx = 0;
	dy = 0;
	move_spd = _move_spd;
}

Player::~Player() {
	// Nothing player-specific to destroy
}

// Update player state
void Player::update() {
	updateSprite();
	bool Collided = false;
	int dx = 0;
	int dy = 0;

	//hacked parameters to determine where x/y should be on collision
	double this_x = x + width()*.25;
	double this_y = y + height()*.75;
	double this_height = height()/4.0; //4.0 - hack to shrink bounding box
	double this_width = width()/2.0;    // to the right size and ratio
	double slope = this_height/this_width;
	double antislope = this_width/this_height;

	// Set velocities according to user input
	Uint8* keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_UP] && dy == 0 && midjump == false) {
		dy = -move_spd;
		y += dy;
		// Initiate walking animation
		startSprite(1);
	}
	else if (keystate[SDLK_DOWN]) {
		dy = move_spd;
		y += dy;
		// Initiate walking animation
		startSprite(1);
	}
	if (keystate[SDLK_LEFT]) {
		dx = -move_spd;
		x += dx;
		// Initiate walking animation
		startSprite(1);
	}
	else if (keystate[SDLK_RIGHT]) {
		dx = move_spd;
		x += dx;
		// Initiate walking animation
		startSprite(1);
	}
	if (!keystate[SDLK_UP] && !keystate[SDLK_DOWN] && !keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]) {
		// Otherwise do the standing animation
		startSprite(0);
	}
    vector<Object*>& level = window->obstacles;
    for (size_t i = 0; i < level.size(); i++) {
        if (checkCollision(*level[i])) {
			Collided = true;
			break;
		}
    }
	if(!Collided) {
		prevX = x;
		prevY = y;
	} else {
		x = prevX;
		y = prevY;
		/*
		if(dx < 0) {
			if(this_y+this_height/2.0 > level[i]->y+(*level[i]).height()/2.0) { //line b collides with player's side c
				//             intercept of the collided object not player even though they are the same on collision
				x = -(*level[i]).getTopA_Yintercept()*antislope + this_width/2.0 + antislope*this_y - width()*.25; //width hack unconversion
			} else { //if (this_y < level[i]->y) { //line d collides with players side a
				x = (*level[i]).getBotB_Yintercept()*antislope - this_width/2.0 - antislope*this_y - width()*.25;
			}
		} else if (dx > 0){ // dx > 0 going to the right
			if(this_y+this_height/2.0 > level[i]->y+(*level[i]).height()/2.0) { //line a collides with player's side d
				x = (*level[i]).getBotA_Yintercept()*antislope - this_width*(3.0/2.0) - antislope*this_y - width()*.25;
			} else {//if (this_y < level[i]->y) { //line c collides with players side b
				x = -(*level[i]).getTopB_Yintercept()*antislope - this_width/2.0 + antislope*this_y - width()*.25;
			}
		}
		if(dy < 0) {
			if(this_x+this_width/2.0 > level[i]->x+(*level[i]).width()/2.0) { //line b collides with player's side c
				y = slope*this_x - this_height/2.0 + (*level[i]).getTopA_Yintercept() - height()*.75; //the height hack unconversion
			} else {//if (x < level[i]->x) { //line d collides with players side a
				y =  -slope*this_x - this_height/2.0 + (*level[i]).getBotB_Yintercept()- height()*.75;
			}
		} else if(dy > 0) { // dy > 0 going up
			if(this_x+this_width/2.0 > level[i]->x+(*level[i]).width()/2.0) { //line a collides with player's side d
				y = -slope*this_x - (3.0/2.0)*this_height + (*level[i]).getBotA_Yintercept() - height()*.75;
			} else {//if (this_x < level[i]->x) { //line c collides with players side b
				y = slope*this_x + this_height/2.0 + (*level[i]).getTopB_Yintercept() - height()*.75;
			}
		}*/
	}

}
