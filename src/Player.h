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
	// Velocity and acceleration variables
	int move_speed;
	int jump_speed;
	int weight;
public:
	Player(int _x, int _y, int move, int jump, int wgt, string imageFile);
	virtual ~Player();
	//virtual void setWindow(GameWindow* win);

	virtual void update();
};

#endif // PLAYER_H
