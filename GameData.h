#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "Includes.h"

/*SUBMODULES OF GAMEDATA*/
#include "InputHandler.h"
#include "PhysicsHandler.h"
#include "CharacterHandler.h"

enum GameMode {INGAME, EDITOR};
enum GameTileIDEnum {AIR, BRICK, GRASS};

struct GameTile : public PhysicsObject{

	GameTileIDEnum tileID;
	bool isSolid;
	bool visible;

	GameTile(PhysicsHandler* physicsHandler, GameTileIDEnum tileID, Vec2 tilePosition, bool isSolid, bool visible);
};

struct GameLevel {
	std::vector<GameTile*> levelData;
};

class GameData {
	Core* core;
	bool editMode_onetime;
	GameTile* debugTile;
public:
	InputHandler* inputHandler;
	PhysicsHandler* physHandler;
	CharacterHandler* characterHandler;

	GameMode currentGameMode;
	GameLevel currentLevel;

	int init();
	int update(float deltaTime);

	GameData(Core* core);
};

#endif