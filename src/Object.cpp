#include <fstream>
#include "SDL_headers.h"

#include "Object.h"
#include "GameWindow.h"
#include "globals.h"

using namespace std;

Object::Object(int _x, int _y) {
	x = _x;
	y = _y;
	surface = NULL;
	surfaceIsShared = false;
	window = NULL;
	anim_num = -1;
	frame_num = 0;
	frame_duration = 0;
}

Object::~Object() {
	if (!surfaceIsShared) {
		SDL_FreeSurface(surface);
	}
}

int Object::width() const {
	if (surface == NULL) return 0;

	if (anim_num < 0)
		return surface->w;
	else
		return sprites[anim_num][frame_num].area.w;
}

int Object::height() const {
	if (surface == NULL) return 0;

	if (anim_num < 0)
		return surface->h;
	else
		return sprites[anim_num][frame_num].area.h;
}

void Object::setWindow(GameWindow* win) {
	window = win;
	// Now that we know the window we should reformat the object surface
	// to match the window's surface for faster blitting.
	if (surface != NULL && !surfaceIsShared) {
		SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
		SDL_FreeSurface(surface);
		surface = temp;
	}
}

void Object::setSurface(SDL_Surface* image, bool _surfaceIsShared) {
	// Delete previous surface and set image as surface
	if (!surfaceIsShared) {
		SDL_FreeSurface(surface);
	}
	surface = image;
	surfaceIsShared = _surfaceIsShared;

	// Convert surface to window format if window is known
	if (window != NULL && surface != NULL && !surfaceIsShared) {
		SDL_Surface* temp = SDL_DisplayFormatAlpha(surface);
		SDL_FreeSurface(surface);
		surface = temp;
	}
}

// Load sprite data
// Returns 0 on success, -1 on failure, and > 0 on partial load
int Object::loadSprite(string filename) {
	ifstream fin(filename.c_str());
	if (!fin.is_open()) return -1;

	// Load data
	int load_status = 0;
	string line;
	while (getline(fin, line)) {
		// Support comments
		if (line[0] == '#') continue;

		vector<string> tokens;
		tokenize(line, tokens);
		// If anim keyword appears, queue up a new sprite animation
		if (tokens.size() >= 2 && tokens[0] == "sprite_sheet") {
			setSurface(IMG_Load(tokens[1].c_str()));
			if (surface == NULL) {
				load_status = 2;
			}
		}
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
		else if (tokens.size() > 0) {
			load_status = 1;
		}
	}

	return load_status;
}

// Start sprite animation designated by num
// Setting num < 0 turns off sprite animation
// Setting num to the current animation does nothing
void Object::startSprite(int num) {
	if (anim_num == num) return;

	if (num < 0) {
		anim_num = -1;
	}
	else if (num >= 0 && num < sprites.size()) {
		anim_num = num;
		frame_num = 0;
		frame_duration = 0;
	}
}

void Object::updateSprite() {
	// Do nothing if sprite does not exist or no animation has been started
	if (anim_num < 0) return;

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

// Update function that updates object state
void Object::update() {
	updateSprite();
}

// Draw function that blits object surface to the window
void Object::draw() {
	if (surface == NULL) return;

	SDL_Rect drawRect = SDL_CreateRect(x, y);
	// If no sprites, paint entire surface
	if (anim_num < 0) {
		SDL_BlitSurface(surface, NULL, window->getSurface(), &drawRect);
	}
	// If sprites are defined, paint sprite area only
	else {
		SDL_BlitSurface(surface, &sprites[anim_num][frame_num].area, window->getSurface(), &drawRect);
	}
}
