#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Includes.h"

struct InputActivity {
	bool forward = false;
	bool back = false;
	bool jump = false;
	bool debug = false;
	bool reset = false;
	bool editMode = false;

	Vec2 mousePos_current;
	Vec2 mousePos_relative;
};

class InputHandler {
public:
	InputActivity inputActivity;

	int update(SDL_Event &e);
	void updateMouseInfo(Vec2 playerPosition, Core* core); // Must be called every frame.
};

#endif