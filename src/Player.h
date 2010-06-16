#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>

#include "Object.h"

using namespace std;

class Player : public Object {
private:

public:
	Player(int _x, int _y, int _rot);
	virtual ~Player();

	virtual void update();
	virtual void draw();
};

#endif // PLAYER_H
