#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Includes.h"

/*SUBMODULES OF GAMEDATA*/
#include "InputHandler.h"
#include "PhysicsHandler.h"
#include "CharacterHandler.h"

enum GameTileIDEnum {AIR, BRICK};

struct GameTile : public PhysicsObject{

	GameTileIDEnum tileID;
	bool isSolid;

	GameTile(PhysicsHandler* physicsHandler, GameTileIDEnum tileID, bool isSolid, b2Vec2 tilePosition);
};

struct GameLevel {
	std::vector<GameTile*> levelData;
};

struct GameData {
	InputHandler* inputHandler;
	PhysicsHandler* physHandler;
	CharacterHandler* characterHandler;
	GameLevel currentLevel;

	int init();
	int update(float deltaTime);

	GameData();
};

#endif