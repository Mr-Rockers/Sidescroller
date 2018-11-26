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
	GameTexture* tiles_grass;
	GameTexture* player_arm;
	GameTexture* player_body;
	GameTexture* player_head;
	GameTexture* player_leg;
	GameTexture* effect_skyDither;
	GameTexture* effect_playerAmbience;
	GameTexture* gui_cogwheel;
	GameTexture* fromGameTileIDEnum(GameTileIDEnum id);
};

class Renderer {
	Core* core;

	GameData* gameData;
	void renderCharacter();
	void renderLimb(SDL_Texture* limbTexture, int playerTexScale, int playerMapScale, int limbHorizontalPoint, int yOffset, float tension, float rotation, bool direction);
	
	SDL_Texture* skyMap;
	void generateSkyMap(); //Generate a black and white map that can be colour keyed.
	void renderSky(SDL_Texture* target, SDL_Colour a, SDL_Colour b);
	void renderDitherLine(SDL_Colour a, SDL_Colour b, float ditherHeight, int scale);

	float cogwheelRotation;

public:
	SDL_Renderer* gameRenderer;

	SDL_Texture* gameSurfaceMap;
	SDL_Texture* gameLightMap;
	SDL_Texture* gamePlayerMap; //Player texture.

	BaseTextures baseTextures;

	float trailOffset;

	Uint8 lightingBrightness; //How bright does the scene get?
	Uint8 lightingGamma;		//How dark does the scene get?

	int init();
	int update(float deltaTime);

	void renderTile(float x, float y, GameTile* tile);
	void generateGradient(SDL_Texture* tex, SDL_Colour a, SDL_Colour b, bool dir);

	float getTrailAmount();
	
	Renderer(GameData* gameData, Core* core);
};


#endif