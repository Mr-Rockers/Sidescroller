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
		checkKey(e, SDL_SCANCODE_SLASH, this->inputActivity.editMode);
	}

	return 0;
}

void InputHandler::updateMouseInfo(Vec2 playerPosition, Core* core) {
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	this->inputActivity.mousePos_current = Vec2(
		(float)mouseX / (float)core->windowWidth,
		(float)mouseY / (float)core->windowHeight
	);

	//CALCULATE THE RELATIVE MOUSE POSITION
	//NOTE THAT THE TRAILOFFSET VARIABLE IS NOT FACTORED IN. SHOULD IT?
	this->inputActivity.mousePos_relative = Vec2(
		((float)mouseX / (float)core->getTileScale()) - ((float)core->windowWidth / (float)core->getTileScale() / 2.0f) + playerPosition.x,
		((float)core->windowHeight / (float)core->getTileScale() / 2.0f) - ((float)(mouseY - core->getHeightAdjustment()) / (float)core->getTileScale()) + playerPosition.y
	);
}
