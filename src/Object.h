#ifndef HEADER_11C442C6F90CC17E
#define HEADER_11C442C6F90CC17E

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "SDL_headers.h"

using namespace std;

class GameWindow;

/* Object Class: Generic object class that all game objects have to inherit from.
   Supports bounding box collisions and sprite animations.
*/

struct SpriteFrame {
	SDL_Rect area;
	unsigned int duration;
	bool isCollidable;
};

class Object {
protected:
	// Surface containing the object's appearance
	shared_ptr<SDL_Surface> surface;
	// Window this object is registered to
	GameWindow* window;
	bool isCollidable;

	// Sprite data
	vector<vector<SpriteFrame>> sprites;
	int anim_num;
	int frame_num;
	// The amount of time the frame has been displayed
	Uint32 frame_duration;

public:
	// Position variables
	int x;
	int y;
	// Isometric y-intercepts
	double topA;
	double topB;
	double botA;
	double botB;

	Object(int _x, int _y);
	virtual ~Object();

	int width() const;
	int height() const;

	virtual void setWindow(GameWindow* win);
	virtual void setSurface(shared_ptr<SDL_Surface> image);

	virtual int loadSprite(string filename);
	virtual void startSprite(int num);
	virtual void updateSprite();

	virtual bool checkCollision(Object& obj) const;
	// For isometric calculations
	double getTopA_Yintercept();
	double getTopB_Yintercept();
	double getBotA_Yintercept();
	double getBotB_Yintercept();

	virtual void update();
	virtual void draw();
};

#endif // header guard 
