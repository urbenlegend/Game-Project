#pragma once

#include <vector>
#include <string>

#include <SDL.h>

using namespace std;

struct SpriteFrame {
	SDL_Rect area;
	unsigned int duration;
};

class Sprite {
private:
	// Sprite data
	SDL_Surface* sprite_sheet;
	vector<vector<SpriteFrame> > sprites;
	int anim_num;
	int frame_num;
	// The amount of time the frame has been displayed
	Uint32 frame_duration;

public:
	Sprite(void);
	~Sprite(void);

	void optimizeSurface();

	int load(string filename);
	void startAnim(int num);

	void update();
	void draw(SDL_Surface* canvas, SDL_Rect drawRect);

	int width();
	int height();
};

