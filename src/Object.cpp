#include <fstream>
#include "SDL_headers.h"

#include "Object.h"
#include "GameWindow.h"
#include "util.h"

using namespace std;

Object::Object(int _x, int _y) {
	x = _x;
	y = _y;
	surface = NULL;
	window = NULL;
	anim_num = -1;
	frame_num = 0;
	frame_duration = 0;
	isCollidable = false;
}

Object::~Object() {

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

double Object::getTopA_Yintercept() {
	double slope = (double)height()/(double)width();
	return (y - (slope*x) - height()/2.0);
}
double Object::getTopB_Yintercept() {
	double slope = (double)height()/(double)width();
	return (y - (slope*x) + height()/2.0);
}
double Object::getBotA_Yintercept() {
	double slope = (double)height()/(double)width();
	return (y + (slope*x) + height()/2);
}
double Object::getBotB_Yintercept() {
	double slope = (double)height()/(double)width();
	return (y + (slope*x) + ((3.0/2.0)*height()));
	/*	double slope = (double)height()/(double)width();
	double TopA2 = y - (slope*x) - height()/2.0;
	double TopB2 = y - (slope*x) + height()/2.0;
	double BottomA2 = y + (slope*x) + height()/2;
	double BottomB2 = y + (slope*x) + ((3.0/2.0)*height());*/
}

void Object::setWindow(GameWindow* win) {
	window = win;
}

void Object::setSurface(shared_ptr<SDL_Surface> image) {
	surface = image;
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
			// TODO: Set deleter to SDL_FreeSurface
			setSurface(shared_ptr<SDL_Surface>(IMG_OptimizedLoadAlpha(tokens[1].c_str()), [](SDL_Surface* for_del) {SDL_FreeSurface(for_del);}));
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
			/*just a hack..we need to probably redesign tiles and their
			  surface relation - spsim */
			if(tokens.size() >= 6 && tokens[5] == "*") {
				frame.isCollidable = true;
			} else {
				frame.isCollidable = false;
			}
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
	//more hacks to get this thing working..
	if(sprites[anim_num][frame_num].isCollidable) {
		window->addObstacle(this);
	} else {
		//probably need to remove the object from obstacle list..
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
// Modified for isometric purposes. I made up an algorithm using an imaginary
// "Y Intercept" value and compares these values to each other.
// Returns true on collision
inline bool Object::checkCollision(Object& obj) const {
	// Bounding extents for this object
	/*if (!obj.isCollidable) {
		return false;
	}*/
	double this_x = x + width()*.25;
	double this_y = y + height()*.75;
	double this_height = height()/4.0; //2.0 - hack to shrink bounding box
	double this_width = width()/2.0;
	double obj_x = obj.x;
	double obj_y = obj.y;
	double obj_height = obj.height();
	double obj_width = obj.width();

	double slope = this_height/this_width;
	double TopA = this_y - (slope*this_x) - this_height/2;
	double TopB = this_y - (slope*this_x) + this_height/2;
	double BottomA = this_y + (slope*this_x) + this_height/2;
	double BottomB = this_y + (slope*this_x) + ((3.0/2.0)*this_height);
	// Bounding extents for obj
	double slope2 = obj_height/obj_width;
	double TopA2 = obj_y - (slope2*obj_x) - obj_height/2;
	double TopB2 = obj_y - (slope2*obj_x) + obj_height/2;
	double BottomA2 = obj_y + (slope2*obj_x) + obj_height/2;
	double BottomB2 = obj_y + (slope2*obj_x) + ((3.0/2.0)*obj_height);

	if (TopA > TopB2) return false;
	if (TopB < TopA2) return false;
	if (BottomA > BottomB2) return false;
	if (BottomB < BottomA2) return false;

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
		SDL_BlitSurface(surface.get(), NULL, window->getSurface(), &drawRect);
	}
	// If sprites are defined, paint sprite area only
	else {
		SDL_BlitSurface(surface.get(), &sprites[anim_num][frame_num].area, window->getSurface(), &drawRect);
	}
}
