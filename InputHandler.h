#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Includes.h"

struct InputActivity {
	bool forward = false;
	bool back = false;
	bool jump = false;
	bool debug = false;
	bool reset = false;
};

class InputHandler {
public:
	InputActivity inputActivity;

	int update(SDL_Event &e);
};

#endif