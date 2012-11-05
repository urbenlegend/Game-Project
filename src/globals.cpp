#include "SDL_headers.h"
#include "Assets.h"

// Global game variables
int cg_fps = 60;

// Assets
AssetManager<SDL_Surface> textures([](SDL_Surface* for_del) {SDL_FreeSurface(for_del);});
