#include "GameData.h"

GameTile::GameTile(PhysicsHandler* physicsHandler, GameTileIDEnum tileID, Vec2 tilePosition, bool isSolid, bool visible) :
PhysicsObject(physicsHandler, tilePosition, Vec2(1.0f, 1.0f), !isSolid),
tileID(tileID),
isSolid(isSolid),
visible(visible){
}

int GameData::init() {
	for (int i = 0; i < 3; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, Vec2(0.0f, (float)(-i) - 1.0f), true, true));
	}
	for (int i = 0; i < 90; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, BRICK, Vec2(-40.0f + (float(i)), -16.0f), true, true));
	}
	for (int i = 0; i < 100; i++) {
		currentLevel.levelData.push_back(new GameTile(this->physHandler, GRASS, Vec2(-50.0f + (float(i)), -20.0f), true, true));
	}
	currentLevel.levelData.push_back(this->debugTile);
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

			if (!this->editMode_onetime && this->inputHandler->inputActivity.editMode) {
				
				if (this->currentGameMode == GameMode::INGAME) {
					this->currentGameMode = GameMode::EDITOR;
				}
				else if (this->currentGameMode == GameMode::EDITOR){
					this->currentGameMode = GameMode::INGAME;
				}

				this->editMode_onetime = true;
			}
			else if (this->editMode_onetime && !this->inputHandler->inputActivity.editMode) {
				this->editMode_onetime = false;
			}
		}

	}

	/*MOUSE INPUT*/
	{
		this->inputHandler->updateMouseInfo(this->characterHandler->position_smooth, this->core);
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

	this->debugTile->visible = this->currentGameMode == GameMode::EDITOR;
	this->debugTile->position = this->inputHandler->inputActivity.mousePos_relative;

	return 0;
}

GameData::GameData(Core* core) : core(core) {
	this->inputHandler = new InputHandler();
	this->physHandler = new PhysicsHandler();
	this->characterHandler = new CharacterHandler(this->inputHandler, this->physHandler, Vec2(1.0f, 10.0f));
	this->currentGameMode = GameMode::INGAME;
	this->editMode_onetime = false;

	this->debugTile = new GameTile(this->physHandler, GameTileIDEnum::BRICK, Vec2(0, 0), false, false);
}
