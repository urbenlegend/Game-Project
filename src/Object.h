#pragma once

#include <string>
#include <vector>
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
	SDL_Surface* surface;
	// Variable indicating whether surface is shared with anything else. If so, do not free it in destructor
	bool surfaceIsShared;
	bool isCollidable;
	// Window this object is registered to
	GameWindow* window;

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
	float topA;
	float topB;
	float botA;
	float botB;
	
	
	//for isometric calculations
	float getTopA_Yintercept();
	float getTopB_Yintercept();
	float getBotA_Yintercept();
	float getBotB_Yintercept();


	void updateTopA_intercept();
	void updateTopB_intercept();
	void updateBotA_intercept();
	void updateBotB_intercept();

	Object(int _x, int _y);
	virtual ~Object();

	int width() const;
	int height() const;

	virtual void setWindow(GameWindow* win);
	virtual void setSurface(SDL_Surface* image, bool _surfaceIsShared = false);

	virtual int loadSprite(string filename);
	virtual void startSprite(int num);
	virtual void updateSprite();

	virtual bool checkCollision(Object& obj) const;

	virtual void update();
	virtual void draw();
};
