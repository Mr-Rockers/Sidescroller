#include "GameData.h"

GameTile::GameTile(PhysicsHandler* physicsHandler, GameTileIDEnum tileID, bool isSolid, b2Vec2 tilePosition) :
PhysicsObject(physicsHandler, tilePosition, b2Vec2(1.0f, 1.0f)),
tileID(tileID),
isSolid(isSolid) {
}

int GameData::init() {
	for (int i = 0; i < 5; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, true, b2Vec2((float)(i), 0.0f) ));
	}
	for (int i = 0; i < 3; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, true, b2Vec2(0.0f, (float)(-i) - 1.0f) ));
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, true, b2Vec2(5.0f, (float)(-i) - 1.0f) ));
	}
	for (int i = 0; i < 100; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, true, b2Vec2(-50.0f + (float(i)), -20.0f)));
	}
	//currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, true, b2Vec2(1.0f, -10.0f)));
	return 0;
}

int GameData::update(float deltaTime) {

	/*EVENTS*/
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return 1;
		}

		/*INPUT*/
		{
			int success = this->inputHandler->update(e);
			if (success != 0) {
				return success;
			}
		}

	}

	/*PHYSICS HANDLER*/
	{
		int success = this->physHandler->update(deltaTime);
		if (success != 0) {
			return success;
		}
	}

	/*CHARACTER HANDLER*/
	{
		int success = this->characterHandler->update(deltaTime);
		if (success != 0) {
			return success;
		}
	}

	if (this->inputHandler->inputActivity.reset) {
	}

	return 0;
}

GameData::GameData() {
	this->inputHandler = new InputHandler();
	this->physHandler = new PhysicsHandler();
	this->characterHandler = new CharacterHandler(this->inputHandler, this->physHandler, b2Vec2(1.0f, 10.0f));
}
