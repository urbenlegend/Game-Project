#include <string>
#include <vector>
#include "SDL_headers.h"

using namespace std;

// Splits a string into several tokens separated by any character in the
// delimiter string. Stores tokens into a vector array for easy access
void tokenize(const string line, vector<string>& tokens, string delimiter) {
	// Clear tokens first
	tokens.clear();

	// Substring positions
	size_t begin = 0;
	size_t end = 0;

	while (end < line.length()) {
		// Extract token into temp
		end = line.find_first_of(delimiter, begin);
		string temp = line.substr(begin, end - begin);

		// Put temp into vector if it isn't an empty string
		if (temp.length() != 0) {
			tokens.push_back(temp);
		}

		begin = end + 1;
	}
}

// Cap fps
void cap_fps(Uint32 start, Uint32 fps) {
	Uint32 delay = 1000 / fps;
	if (SDL_GetTicks() - start < delay)
		SDL_Delay(delay - (SDL_GetTicks() - start));
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

SDL_Rect SDL_CreateRect(int x, int y, int w = 0, int h = 0) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

SDL_Surface* IMG_OptimizedLoadAlpha(const char* filename) {
	SDL_Surface* loadedImage = IMG_Load(filename);
	SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
	SDL_FreeSurface(loadedImage);
	return optimizedImage;
}
