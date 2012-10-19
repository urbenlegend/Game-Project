#pragma once

#include <string>
#include <vector>
#include "SDL_headers.h"

using namespace std;

// Global variables declarations
extern int cg_fps;
extern double g_axfactor;

// Global helper functions

// Splits a string into several tokens separated by any character in the
// delimiter string. Stores tokens into a vector array for easy access
void tokenize(const string line, vector<string>& tokens, string delimiter = " \t");

// Cap fps
void cap_fps(Uint32 start, Uint32 fps);

double interpolate(double t1, double t2, double p1, double p2);

SDL_Rect SDL_CreateRect(int x, int y, int w = 0, int h = 0);

SDL_Surface* IMG_OptimizedLoadAlpha(const char* filename);
