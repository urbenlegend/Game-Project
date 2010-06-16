#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

inline void cap_fps(Uint32 start, Uint32 fps) {
	// Cap fps
	if (SDL_GetTicks() - start < 1000 / fps)
		SDL_Delay((1000 / fps) - (SDL_GetTicks() - start));
}

double interpolate(double t1, double t2, double p1, double p2) {
	if (SDL_GetTicks() < t1)
		return p1;
	else if (SDL_GetTicks() > t2)
		return p2;
	else {
		double alpha = (SDL_GetTicks() - t1) / (t2 - t1);
		return (1 - alpha)*(p1) + alpha*(p2);
	}
}

inline SDL_Surface* IMG_OptimizedLoadAlpha(const char* filename) {
	SDL_Surface* loadedImage = IMG_Load(filename);
	SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
	SDL_FreeSurface(loadedImage);
	return optimizedImage;
}

#endif // HELPERS_H_INCLUDED
