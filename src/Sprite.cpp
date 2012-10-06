#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>

#include "globals.h"
#include "Sprite.h"

using namespace std;

Sprite::Sprite()
{
	anim_num = -1;
	frame_num = -1;
	frame_duration = -1;
}

Sprite::~Sprite()
{
	SDL_FreeSurface(sprite_sheet);
}

void Sprite::optimizeSurface() {
	SDL_Surface* temp = SDL_DisplayFormatAlpha(sprite_sheet);
	SDL_FreeSurface(sprite_sheet);
	sprite_sheet = temp;
}

// Load sprite data
// Returns 0 on success, -1 on failure, and > 0 on partial load
int Sprite::load(string filename) {
	ifstream fin(filename.c_str());
	if (!fin.is_open()) return -1;

	// Load data
	int load_status = 0;
	string line;
	while (getline(fin, line)) {
		// Support comments
		if (line[0] == '#') continue;

		vector<string> tokens;
		split_tokens(line, tokens);
		if (tokens.size() >= 2 && tokens[0] == "sprite_sheet") {
			sprite_sheet = IMG_Load(tokens[1].c_str());
			if (sprite_sheet == NULL) {
				load_status = 2;
			}
		}
		// If anim keyword appears, queue up a new sprite animation
		else if (tokens.size() > 0 && tokens[0] == "anim") {
			vector<SpriteFrame> new_anim;
			sprites.push_back(new_anim);
		}
		// else read frame data and add to current sprite animation
		else if (tokens.size() >= 5 && sprites.size() > 0) {
			SpriteFrame frame;
			frame.area.x = atoi(tokens[0].c_str());
			frame.area.y = atoi(tokens[1].c_str());
			frame.area.w = atoi(tokens[2].c_str());
			frame.area.h = atoi(tokens[3].c_str());
			frame.duration = atoi(tokens[4].c_str());
			sprites.back().push_back(frame);
		}
		else {
			load_status = 1;
		}
	}

	return load_status;
}

// Start sprite animation designated by num
// Setting num to -1 turns off sprite animation
// Setting num to the current animation does nothing
void Sprite::startAnim(int num) {
	if (num == -1) {
		anim_num = -1;
		frame_num = -1;
		frame_duration = -1;
	}
	else if (anim_num != num && num < sprites.size()) {
		anim_num = num;
		frame_num = 0;
		frame_duration = 0;
	}
}

void Sprite::update() {
	// Do nothing if sprite does not exist or no animation has been started
	if (anim_num == -1 || frame_num == -1) return;

	// Move to next frame if previous frame has exceeded display duration
	if (frame_duration > sprites[anim_num][frame_num].duration) {
			if (frame_num < sprites[anim_num].size() - 1) {
				frame_num++;
			}
			else {
				frame_num = 0;
			}
			frame_duration = 0;
	}
	else {
		frame_duration++;
	}
}

void Sprite::draw(SDL_Surface* canvas, SDL_Rect drawRect) {
	SDL_BlitSurface(sprite_sheet, &sprites[anim_num][frame_num].area, canvas, &drawRect);
}

int Sprite::width() {
	return sprites[anim_num][frame_num].area.w;
}

int Sprite::height() {
	return sprites[anim_num][frame_num].area.h;
}