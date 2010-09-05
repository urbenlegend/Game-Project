#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <SDL/SDL.h>

using namespace std;

class GameWindow;

/* Object Class: Generic object class that all game objects have to inherit from.
   Supports bounding box collisions and sprite animations.
*/
class Object {
protected:
	// Surface containing the object's appearance
	SDL_Surface* surface;
	// Window this object is registered to
	GameWindow* window;

	// Sprite data
	struct SpriteFrame {
		SDL_Rect area;
		unsigned int duration;
	};
	vector<vector<SpriteFrame>> sprites;
	int anim_num;
	int frame_num;
	// The amount of time the frame has been displayed
	Uint32 frame_duration;


	// Position and velocity variables
	int dx;
	int dy;
public:
	int x;
	int y;

	Object(SDL_Surface* image, int _x, int _y);
	virtual ~Object();

	int width() const;
	int height() const;

	virtual void setWindow(GameWindow* win);
	virtual void setSurface(SDL_Surface* image);
	virtual int loadSprite(string filename);

	virtual bool checkCollision(Object& obj) const;

	virtual void startSprite(int num);
	virtual void update();
	virtual void draw();
};

// Functions to access surface attributes
inline int Object::width() const {
	if (anim_num == -1 || frame_num == -1)
		return surface->w;
	else
		return sprites[anim_num][frame_num].area.w;
}
inline int Object::height() const {
	if (anim_num == -1 || frame_num == -1)
		return surface->h;
	else
		return sprites[anim_num][frame_num].area.h;
}

#endif // OBJECT_H
