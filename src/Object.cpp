#include <fstream>
#include <SDL/SDL.h>

#include "Object.h"
#include "GameWindow.h"
#include "helpers.h"

Object::Object(SDL_Surface* image, int _x, int _y) {
	surface = image;
	window = NULL;
	x = _x;
	y = _y;
	dx = 0;
	dy = 0;
	anim_num = -1;
	frame_num = -1;
	frame_duration = -1;
}

Object::~Object() {
	SDL_FreeSurface(surface);
}

void Object::setWindow(GameWindow* win) {
	window = win;
	// Now that we know the window we should reformat the object surface
	// to match the window's surface for faster blitting.
	SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
	SDL_FreeSurface(surface);
	surface = temp;
}

void Object::setSurface(SDL_Surface* image) {
	// Delete previous surface and set image as surface
	SDL_FreeSurface(surface);
	surface = image;

	// Convert surface to window format if window is known
	if (window != NULL) {
		SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
		SDL_FreeSurface(surface);
		surface = temp;
	}
}

// Load sprite data
// Returns 0 on success, -1 on failure, and > 0 on partial load
int Object::loadSprite(string filename) {
	ifstream fin(filename);
	if (!fin.is_open()) return -1;

	// Load data
	int load_status = 0;
	string line;
	while (getline(fin, line)) {
		// Support comments
		if (line[0] == '#') continue;

		vector<string> tokens;
		split_tokens(line, tokens);
		// If anim keyword appears, queue up a new sprite animation
		if (tokens.size() > 0 && tokens[0] == "anim") {
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


// Check for collision between two objects using basic bounding boxes
inline bool Object::checkCollision(Object& obj) const {
	// Bounding extents for this object
	int left1 = x;
	int right1 = x + width() - 1;
	int top1 = y;
	int bottom1 = y + height() - 1;
	// Bounding extents for obj
	int left2 = obj.x;
	int right2 = obj.x + obj.width() - 1;
	int top2 = obj.y;
	int bottom2 = obj.y + obj.height() - 1;

	if (left1 > right2) return false;
	if (right1 < left2) return false;
	if (top1 > bottom2) return false;
	if (bottom1 < top2) return false;

	return true;
}

// Start sprite animation designated by num
void Object::startSprite(int num) {
	if (num == -1) {
		anim_num = -1;
		frame_num = -1;
		frame_duration = -1;
	}
	else if (num < sprites.size()) {
		anim_num = num;
		frame_num = 0;
		frame_duration = 0;
	}
}

// Update function that updates object state
void Object::update() {
	// Apply dx and dy
	x += dx;
	y += dy;
}

// Draw function that blits object surface to the window
void Object::draw() {
	SDL_Rect drawRect = SDL_CreateRect(x, y);
	// If no sprites, paint entire surface
	if (anim_num == -1 || frame_num == -1) {
		SDL_BlitSurface(surface, NULL, window->getSurface(), &drawRect);
	}
	// If sprites are defined, paint sprite area only
	else {
		if (frame_duration > sprites[anim_num][frame_num].duration) {
			if (frame_num < sprites[anim_num].size() - 1) {
				frame_num++;
			}
			else {
				frame_num = 0;
			}
			frame_duration = 0;
		}
		SDL_BlitSurface(surface, &sprites[anim_num][frame_num].area, window->getSurface(), &drawRect);
		frame_duration++;
	}
}
