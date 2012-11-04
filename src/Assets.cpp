#include "SDL_headers.h"

#include "Assets.h"

Texture::Texture() {
	surface = NULL;
	freeOnDestroy = true;
}
// Non-move constructor for cases where you don't want a Texture
// to own a SDL_Surface, but simply wrap around it
Texture::Texture(SDL_Surface* const& _surface) {
	surface = _surface;
	freeOnDestroy = false;
}
// Move constructor for when you want to create a Texture
// that owns a SDL_Surface and will eventually destroy it
Texture::Texture(SDL_Surface*&& _surface) {
	surface = _surface;
	freeOnDestroy = true;
	_surface = NULL;
}
// Non-move constructor for when you want to copy a Texture
Texture::Texture(const Texture& other) {
	operator=(other);
}
// Move constructor for when you want to move a Texture
Texture::Texture(Texture&& other) {
	surface = other.surface;
	freeOnDestroy = other.freeOnDestroy;
	other.surface = NULL;
}
// Non-move assignment operator for assigning a SDL_Surface to a Texture
Texture& Texture::operator=(SDL_Surface* const& _surface) {
	destroyTexture();
	surface = _surface;
	freeOnDestroy = false;
	return *this;
}
// Move assignment operator for moving a SDL_Surface to a Texture
Texture& Texture::operator=(SDL_Surface*&& _surface) {
	destroyTexture();
	surface = _surface;
	freeOnDestroy = true;
	_surface = NULL;
	return *this;
}
// Non-move assignment operator for copying from one texture to another
Texture& Texture::operator=(const Texture& other) {
	destroyTexture();
	surface = SDL_ConvertSurface(other.surface, other.surface->format, other.surface->flags);
	freeOnDestroy = true;
	return *this;
}
// Move assignment operator for moving one texture's data to another
Texture& Texture::operator=(Texture&& other) {
	destroyTexture();
	surface = other.surface;
	freeOnDestroy = other.freeOnDestroy;
	other.surface = NULL;
	return *this;
}

Texture::~Texture() {
	destroyTexture();
}

void Texture::destroyTexture() {
	if (freeOnDestroy) {
		SDL_FreeSurface(surface);
	}
}

SDL_Surface* Texture::getSDLSurface() {
	return surface;
}

int Texture::width() {
	return surface->w;
}

int Texture::height() {
	return surface->h;
}

void Texture::draw(Texture& dest, Rect src_rect, Rect dest_rect) {
	SDL_BlitSurface(surface, &src_rect, dest.surface, &dest_rect);
}

Texture& Texture::scale(int xfactor, int yfactor) {
	// Unimplemented
	return *this;
}
