#ifndef EXTRAFUNCTIONS_HELPER_H
#define EXTRAFUNCTIONS_HELPER_H

#include "SDL.h"

/*GENERAL*/
class Core {
	int tileScale;
	int heightAdjustment;
public:
	int windowWidth;
	int windowHeight;
	float trailOffset;
	SDL_Window* mainWindow;

	int getTileScale() {
		return this->tileScale;
	}
	void setTileScale(float widthPercent) {
		this->tileScale = (int)(widthPercent * (float)this->windowWidth);
		this->heightAdjustment = this->tileScale * 2;
	}

	int getHeightAdjustment() {
		return this->heightAdjustment;
	}

	float getTrailAmount() {
		return (float)this->tileScale / 2.0f;
	}

	Core(int windowWidth, int windowHeight) :
		windowWidth(windowWidth),
		windowHeight(windowHeight),
		trailOffset(0.0f),
		mainWindow(nullptr) {
		this->setTileScale(0.05f);
	}
};

enum FIND_TYPE { LARGEST, SMALLEST };
template <typename T>
static unsigned int FindInArray(T* valueArray, unsigned int size, FIND_TYPE type) { //Returns appropriate index.
	unsigned int candidate = 0;
	if (size > 1) {
		for (unsigned int i = 0; i < size; i++) {
			bool updateCandidate = false;
			switch (type) {
			case LARGEST:
				updateCandidate = valueArray[i] > valueArray[candidate];
				break;
			case SMALLEST:
				updateCandidate = valueArray[i] < valueArray[candidate];
				break;
			default:
				break;
			}
			if (updateCandidate) {
				candidate = i;
			}
		}
	}
	return candidate;
}

/*API SPECIFIC*/
static const SDL_Rect GetRect(int w, int h, int x, int y) {
	SDL_Rect foo;
	foo.w = w;
	foo.h = h;
	foo.x = x;
	foo.y = y;
	return foo;
}
static const SDL_Point GetPoint(int x, int y) {
	SDL_Point foo;
	foo.x = x;
	foo.y = y;
	return foo;
}
static const SDL_Colour GetColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_Colour val;
	val.r = r;
	val.g = g;
	val.b = b;
	val.a = a;
	return val;
}
#endif