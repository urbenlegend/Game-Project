#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>

#include "Object.h"

using namespace std;

class Player : public Object {
private:
	// Player image
	SDL_Surface* surface;
	// Physics variables
	int dx;
	int dy;
	bool midjump;
	// Velocity and acceleration variables
	int move_speed;
	int jump_speed;
	int weight;
public:
	Player(int _x, int _y, int _rot, int move, int jump, int wgt);
	virtual ~Player();
	virtual void setWindow(GameWindow* win);

	virtual void update();
	virtual void draw();
};

#endif // PLAYER_H
