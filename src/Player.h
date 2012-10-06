#pragma once

#include <SDL.h>

#include "Object.h"
#include "Sprite.h"

using namespace std;

/* Player Class: Object that represents a player controlled character
 * Has input logic and physics calculations
 */
class Player : public Object {
protected:
	Sprite* playerSprite;
	bool midjump;
public:
	// Physics variables
	double dx;
	double dy;
	double ax;
	double ay;
	double move_spd;
	double jump_spd;

	Player(Sprite* _playerSprite, int _x, int _y, double _move_spd, double _jump_spd, double _ay);
	virtual ~Player();

	virtual void setWindow(GameWindow* win);

	virtual int width() const;
	virtual int height() const;

	virtual void update();
	virtual void draw();
};
