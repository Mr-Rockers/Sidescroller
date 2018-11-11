#ifndef EXTRAFUNCTIONS_HELPER_H
#define EXTRAFUNCTIONS_HELPER_H

/*GENERAL*/
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

#endif