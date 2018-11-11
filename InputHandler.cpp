#include "InputHandler.h"

inline void checkKey(SDL_Event &e, SDL_Scancode scancode, bool &inputVar) {
	if (e.key.keysym.scancode == scancode) {
		inputVar = e.key.state == SDL_PRESSED;
	}
}

int InputHandler::update(SDL_Event &e) {

	if (e.key.state == SDL_PRESSED || e.key.state == SDL_RELEASED) {
		checkKey(e, SDL_SCANCODE_D, this->inputActivity.forward);
		checkKey(e, SDL_SCANCODE_A, this->inputActivity.back);
		checkKey(e, SDL_SCANCODE_SPACE, this->inputActivity.jump);
		checkKey(e, SDL_SCANCODE_GRAVE, this->inputActivity.debug);
		checkKey(e, SDL_SCANCODE_R, this->inputActivity.reset);
	}

	return 0;
}
