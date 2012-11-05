#pragma once

#include "SDL_headers.h"

#include "Object.h"

using namespace std;

/* Player Class: Object that represents a player controlled character
 * Has input logic and physics calculations
 */
class Player : public Object {
protected:
	bool midjump;
public:
	// Physics variables
	double prevX;
	double prevY;
	double dx;
	double dy;
	double move_spd;

	Player(int _x, int _y, double _move_spd);
	virtual ~Player();

	virtual void update();
};
