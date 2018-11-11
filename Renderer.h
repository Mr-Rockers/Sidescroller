#ifndef RENDERER_H
#define RENDERER_H

#include "Includes.h"

struct GameTexture {
	SDL_Texture* surface;
	SDL_Texture* bump;

	static GameTexture* LoadImage(SDL_Renderer* gameRenderer, std::string imageName);
	~GameTexture();
};

struct BaseTextures {
	GameTexture* brick;
	GameTexture* player_arm;
	GameTexture* player_body;
	GameTexture* player_head;
	GameTexture* player_leg;

	GameTexture* fromGameTileIDEnum(GameTileIDEnum id);
};

class Renderer {
	GameData* gameData;
	void renderCharacter();

public:
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;

	SDL_Texture* gameSurfaceMap;
	SDL_Texture* gameLightMap;

	BaseTextures baseTextures;

	int windowWidth = 1280, windowHeight = 720;

	int tileScale = 64;
	int heightAdjustment = windowHeight - tileScale * 2;

	int init();
	int update(float deltaTime);

	void RenderTile(float x, float y, GameTile* tile);

	Renderer(GameData* gameData);
};


#endif