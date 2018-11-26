#include "Includes.h"

/*MODULE*/#include "GameData.h"
/*MODULE*/#include "Renderer.h"

/*BASE CLASSES*/
Core core = Core(1280, 720);
GameData gameData = GameData(&core);
Renderer renderer = Renderer(&gameData, &core);

int run();
int main(int argc, char** argv) {

	/*INITIALISATION PHASE*/
	//Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return -1;
	}

	{
		int success = gameData.init();
		if (success != 0) {
			return success;
		}
	}

	{
		int success = renderer.init();
		if (success != 0) {
			return success;
		}
	}

	return run();
}

Uint32 oldTicks = 0, newTicks = 0;
int run() {
	bool gameActive = true;
	while (gameActive) {

		/*DELTATIME*/
		oldTicks = newTicks;
		newTicks = SDL_GetTicks();
		float deltaTime = ((float)(newTicks)-(float)(oldTicks)) / 1000.0f;

		/*GAMEPLAY*/
		{
			int success = gameData.update(deltaTime);
			if (success < 0) {
				return -1;
			}
			else if (success > 0) {
				gameActive = false;
			}
 		}

		/*GRAPHICS*/
		{
			int success = renderer.update(deltaTime);
			if (success < 0) {
				return -1;
			}
			else if (success > 0) {
				gameActive = false;
			}
		}
	}
	return 0;
}