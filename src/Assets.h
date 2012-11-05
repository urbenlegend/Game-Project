#pragma once

#include <map>
#include <string>
#include <atomic>
#include <mutex>
#include <memory>
#include "SDL_headers.h"

using namespace std;

typedef SDL_Rect Rect;

// Texture class. Abstraction over low-level video surfaces.
// Has copy/move semantics as follows:
// Case 1: Texture wraps a SDL_Surface. Data is shared and surface still needs to be freed independently
// SDL_Surface* surface;
// Texture tex = surface; // Modifying surface modifies tex
//
// Case 2: Texture gains ownership of a SDL_Surface by use of move semantics. tex frees surface data on destruction, so no need to free surface independently
// Texture tex = std::move(surface); // tex now owns surface and invalidates surface pointer.
//
// Case 3: Texture deep copies if initialized or assigned to another Texture
// Texture tex1();
// Texture tex2 = tex1; // tex1 is copied to tex2
//
// Case 4: Texture is moved when forced with move semantics
// Texture tex1();
// Texture tex2 = std::move(tex1); // tex1's data is moved to tex2 and tex1 is invalidated
//
// Of course, moves are also triggered when returning temporaries from functions, so case 2 and 4 apply below:
// Texture tex = returnSurface();
// Texture tex = returnTexture();

class Texture {
private:
	SDL_Surface* surface;
	bool freeOnDestroy;

	void destroyTexture();

public:
	Texture();
	Texture(SDL_Surface* const& _surface);
	Texture(SDL_Surface*&& _surface);
	Texture(const Texture& _other);
	Texture(Texture&& other);
	Texture& operator=(SDL_Surface* const& _surface);
	Texture& operator=(SDL_Surface*&& _surface);
	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other);
	~Texture();

	SDL_Surface* getSDLSurface();
	int width();
	int height();

	void draw(Texture& dest, Rect src_rect, Rect dest_rect);
	Texture& scale(int xfactor, int yfactor);

};

typedef atomic<int> G_Int;
typedef atomic<double> G_Double;

template <typename T>
class AssetManager {
private:
	map<string, shared_ptr<T>> assets;
	map<string, shared_ptr<mutex>> element_locks;
	mutex map_lock;
	void (*deleter)(T*);

public:
	AssetManager<T>(void (*_deleter)(T*) = NULL);

	shared_ptr<T> get(string id);
	shared_ptr<T> operator[](string id);

	void add(string id, T* value = NULL, void (*_del)(T*) = NULL);
	void remove(string id);

	void lock(string id);
	void unlock(string id);
};

template <typename T>
AssetManager<T>::AssetManager(void (*_deleter)(T*)) {
	deleter = _deleter;
}

template <typename T>
shared_ptr<T> AssetManager<T>::get(string id) {
	lock_guard<mutex> lock(map_lock);
	auto itr = assets.find(id);
	if (itr != assets.end()) {
		return itr->second;
	}
	else {
		return NULL;
	}
}

template <typename T>
shared_ptr<T> AssetManager<T>::operator[](string id) {
	return get(id);
}

template <typename T>
void AssetManager<T>::add(string id, T* value, void (*_del)(T*)) {
	lock_guard<mutex> lock(map_lock);
	// Add value to assets, along with an optional deleter
	void (*del)(T*) = _del ? _del : deleter;

	if (del == NULL) {
		assets[id] = shared_ptr<T>(value);
	}
	else {
		assets[id] = shared_ptr<T>(value, del);
	}
	// Add a mutex lock associated with the asset
	element_locks[id] = shared_ptr<mutex>(new mutex());
}

template <typename T>
void AssetManager<T>::remove(string id) {
	lock_guard<mutex> lock(map_lock);
	assets.erase(id);
	element_locks.erase(id);
}

template <typename T>
void AssetManager<T>::lock(string id) {
	lock_guard<mutex> lock(map_lock);
	auto itr = element_locks.find(id);
	if (itr != element_locks.end()) {
		itr->second->lock();
	}
}

template <typename T>
void AssetManager<T>::unlock(string id) {
	lock_guard<mutex> lock(map_lock);
	auto itr = element_locks.find(id);
	if (itr != element_locks.end()) {
		itr->second->unlock();
	}
}

