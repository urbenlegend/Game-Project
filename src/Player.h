#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>

#include "Object.h"

using namespace std;

/* Player Class: Object that represents a player controlled character
 * Has input logic and physics calculations
 */
class Player : public Object {
protected:
	// Physics variables
	int dx;
	int dy;
	bool midjump;
public:
	// Velocity and acceleration variables
	int move_spd;
	int jump_spd;
	int weight;

	Player(string imageFile, int _x, int _y, int _move_spd, int _jump_spd, int _weight);
	virtual ~Player();

	virtual void update();
};

#endif // PLAYER_H
