#pragma once

#include <SDL.h>

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
	double dx;
	double dy;
	double ax;
	double ay;
	double move_spd;
	double jump_spd;

	Player(int _x, int _y, double _move_spd, double _jump_spd, double _ay);
	virtual ~Player();

	virtual void update();
};
