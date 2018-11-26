#include "CharacterHandler.h"
#include "GameData.h"
#include "Renderer.h"

bool renderDebug = false, renderDebug_oneTime = true;


GameTexture* GameTexture::LoadImage(SDL_Renderer* gameRenderer, std::string imageName) {
	GameTexture* texContainer = new GameTexture();
	std::string surfacePath = std::string("assets/textures/surface/").append(imageName).append(".png");
	texContainer->surface = IMG_LoadTexture(gameRenderer, surfacePath.c_str());

	if (texContainer->surface == nullptr) {
		std::cout << SDL_GetError();
		std::cin.get();
		return false; //Will only load texture to register if there is a surface texture.
	}

	std::string bumpPath = std::string("assets/textures/bump/").append(imageName).append(".png");
	texContainer->bump = IMG_LoadTexture(gameRenderer, bumpPath.c_str());
	SDL_SetTextureBlendMode(texContainer->bump, SDL_BLENDMODE_MOD);

	return texContainer;
}
GameTexture::~GameTexture() {
	if (this->surface != nullptr) {
		SDL_DestroyTexture(this->surface);
	}
	if (this->bump != nullptr) {
		SDL_DestroyTexture(this->bump);
	}
}

/*
We need to have a standard.
Therefore, the standard is that all textures must align to the left.

Hence, we now need to know:
The centre point of each limb texture.
(For the head, this would be the 4th px along.)

Where this "centre point" is relative to the
position of the character.
(For instance, the head is the 4th px along relative to the texture
BUT we should make sure this 4th px along is the centre of the character..)
*/
void Renderer::renderCharacter() {
	CharacterHandler* characterHandler = this->gameData->characterHandler;
	int playerTexScale = 20; // This arbritrary to the default assets.
	
	int playerMapScale = 0;
	SDL_QueryTexture(this->gamePlayerMap, nullptr, nullptr, &playerMapScale, nullptr);

	SDL_SetRenderTarget(this->gameRenderer, this->gamePlayerMap);
	SDL_SetRenderDrawColor(this->gameRenderer, 0, 0, 0, 0);
	SDL_RenderClear(this->gameRenderer);
	SDL_SetRenderDrawColor(this->gameRenderer, 255, 255, 255, 255);

	//BACK ARM (Horizontal centre point: 2nd px)
	SDL_SetTextureColorMod(this->baseTextures.player_arm->surface, 128, 128, 200);
	this->renderLimb(
		this->baseTextures.player_arm->surface,
		playerTexScale,
		playerMapScale,
		2,
		18,
		characterHandler->arm_back_tension * 8.5f,
		characterHandler->arm_back_rotation,
		characterHandler->direction
	);
	SDL_SetTextureColorMod(this->baseTextures.player_arm->surface, 255, 255, 255);

	//BACK LEG (Horizontal centre point: 13th px)
	SDL_SetTextureColorMod(this->baseTextures.player_leg->surface, 128, 128, 200);
	this->renderLimb(
		this->baseTextures.player_leg->surface,
		playerTexScale,
		playerMapScale,
		13,
		0,
		characterHandler->leg_back_tension * 4.5f,
		characterHandler->leg_back_rotation,
		characterHandler->direction
	);
	SDL_SetTextureColorMod(this->baseTextures.player_leg->surface, 255, 255, 255);

	//HEAD (Horizontal centre point: 4th px)
	this->renderLimb(
		this->baseTextures.player_head->surface,
		playerTexScale,
		playerMapScale,
		4,
		40,
		0.0f,
		0.0f,
		characterHandler->direction
	);

	//FRONT LEG (Horizontal centre point: 13th px)
	this->renderLimb(
		this->baseTextures.player_leg->surface,
		playerTexScale,
		playerMapScale,
		13,
		0,
		characterHandler->leg_frnt_tension * 4.5f,
		characterHandler->leg_frnt_rotation,
		characterHandler->direction
	);

	//BODY (Horizontal centre point: 3rd px)
	this->renderLimb(
		this->baseTextures.player_body->surface,
		playerTexScale,
		playerMapScale,
		3,
		20,
		0.0f,
		0.0f,
		characterHandler->direction
	);

	//FRONT ARM (Horizontal centre point: 2nd px)
	this->renderLimb(
		this->baseTextures.player_arm->surface,
		playerTexScale,
		playerMapScale,
		2,
		18,
		characterHandler->arm_frnt_tension * 8.5f,
		characterHandler->arm_frnt_rotation,
		characterHandler->direction
	);
}

void Renderer::renderLimb(SDL_Texture* limbTexture, int playerTexScale, int playerMapScale, int limbHorizontalPoint, int yOffset, float tension, float rotation, bool direction) {
	int correctedHorizontalPoint = (playerMapScale / 2) + (direction ? -limbHorizontalPoint : -playerTexScale+limbHorizontalPoint);
	int correctedVerticalPoint = playerMapScale - playerTexScale - yOffset;
	SDL_RenderCopyEx(
		this->gameRenderer,
		limbTexture,
		&GetRect(
			playerTexScale,
			playerTexScale,
			playerTexScale * (int)(tension),
			0
		),
		&GetRect(
			playerTexScale,
			playerTexScale,
			correctedHorizontalPoint,
			correctedVerticalPoint
		),
		rotation * (direction ? -1.0f : 1.0f),
		&GetPoint(direction ? limbHorizontalPoint : playerTexScale-limbHorizontalPoint,0),
		direction ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL
	);
}

void Renderer::generateSkyMap() {

	this->skyMap = SDL_CreateTexture(this->gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, this->core->windowWidth, this->core->windowHeight);
	SDL_SetTextureBlendMode(this->skyMap, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	this->generateGradient(this->skyMap, GetColour(255, 255, 255, 0), GetColour(255, 255, 255, 255), true);
}

void Renderer::renderDitherLine(SDL_Colour a, SDL_Colour b, float ditherHeight, int scale) {
	SDL_SetTextureColorMod(this->baseTextures.effect_skyDither->surface,
		Lerp(a.r, b.r, ditherHeight),// + (scale / this->windowHeight)/2.0f),
		Lerp(a.g, b.g, ditherHeight),// + (scale / this->windowHeight)/2.0f),
		Lerp(a.b, b.b, ditherHeight)// + (scale / this->windowHeight)/2.0f)
	);
	for (int ditherX = 0; ditherX < (this->core->windowWidth / scale) + 1; ditherX++)
		SDL_RenderCopy(this->gameRenderer, this->baseTextures.effect_skyDither->surface, NULL, &GetRect(
			scale,
			scale,
			ditherX * scale,
			(int)((float)this->core->windowHeight * ditherHeight)
		));

}

void Renderer::renderSky(SDL_Texture* target, SDL_Colour a, SDL_Colour b) {
	SDL_Texture* previous = SDL_GetRenderTarget(this->gameRenderer);
	SDL_SetRenderTarget(this->gameRenderer, target);

	SDL_SetRenderDrawColor(this->gameRenderer, a.r, a.g, a.b, 255);
	SDL_RenderClear(this->gameRenderer);
	SDL_SetTextureColorMod(this->skyMap, b.r, b.g, b.b);
	SDL_RenderCopy(this->gameRenderer, this->skyMap, NULL, NULL);

	SDL_SetRenderTarget(this->gameRenderer, previous);
}

int Renderer::init() {
	//Init SDL Image
	int imgInitFlags = IMG_INIT_PNG;
	int imgInitState = IMG_Init(imgInitFlags);
	if ((imgInitState & imgInitFlags) != imgInitFlags) {
		return -1;
	}

	//Create Window & Renderer
	this->core->mainWindow = SDL_CreateWindow("Sidescroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->core->windowWidth, this->core->windowHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(this->core->mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (this->core->mainWindow == nullptr || gameRenderer == nullptr) {
		return -1;
	}

	//Load Resources
	baseTextures.brick = GameTexture::LoadImage(gameRenderer, "bricks");
	baseTextures.tiles_grass = GameTexture::LoadImage(gameRenderer, "tiles/grass");
	baseTextures.player_arm = GameTexture::LoadImage(gameRenderer, "player/arm");
	baseTextures.player_body = GameTexture::LoadImage(gameRenderer, "player/body");
	baseTextures.player_head = GameTexture::LoadImage(gameRenderer, "player/head");
	baseTextures.player_leg = GameTexture::LoadImage(gameRenderer, "player/leg");
	baseTextures.effect_skyDither = GameTexture::LoadImage(gameRenderer, "effects/skyDither");
	baseTextures.effect_playerAmbience = GameTexture::LoadImage(gameRenderer, "effects/playerAmbience");
	baseTextures.gui_cogwheel = GameTexture::LoadImage(gameRenderer, "gui/cogwheel");

	//Generate Textures.
	this->generateSkyMap();
	this->gameSurfaceMap = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, this->core->windowWidth, this->core->windowHeight);
	this->gameLightMap = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, this->core->windowWidth, this->core->windowHeight);
	this->gamePlayerMap = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 500, 500);
	
	//Set Texture Properties.
	SDL_SetTextureBlendMode(this->gamePlayerMap, SDL_BLENDMODE_BLEND);

	return 0;
}

int Renderer::update(float deltaTime) {
	//Handle Debug
	{
		if (this->gameData->inputHandler->inputActivity.debug && renderDebug_oneTime) {
			renderDebug = !renderDebug;
			renderDebug_oneTime = false;
		}
		if (!this->gameData->inputHandler->inputActivity.debug) {
			renderDebug_oneTime = true;
		}
	}

	//Reset Textures
	SDL_SetRenderTarget(this->gameRenderer, this->gameLightMap);
	SDL_SetRenderDrawColor(this->gameRenderer, this->lightingBrightness, this->lightingBrightness, this->lightingBrightness, 255);
	SDL_RenderClear(this->gameRenderer);
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	SDL_SetRenderDrawColor(this->gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(this->gameRenderer);

	//Render Sky
	this->renderSky(this->gameSurfaceMap, GetColour(7, 101, 150, 255), GetColour(255, 255, 255, 255));

	//Render Background
	if (this->gameData->characterHandler->currentAction == IDLE) {
		trailOffset = Lerp(trailOffset, 0.0f, deltaTime * 4.0f);
	}
	else {
		trailOffset += (this->gameData->characterHandler->movement.x / this->gameData->characterHandler->playerSpeed);
		float trailAmount = this->getTrailAmount();
		if (trailOffset > trailAmount) {
			trailOffset = trailAmount;
		}
		if (trailOffset < -trailAmount) {
			trailOffset = -trailAmount;
		}
	}

	Vec2 playerPosition = this->gameData->characterHandler->position_smooth;
	playerPosition.x -= (this->gameData->characterHandler->dimensions.x / 2.0f) * (1.0f / this->gameData->characterHandler->dimensions.x);
	playerPosition.y -= 1.5f; //-ve is down in the physics handler

	for (GameTile* tile : this->gameData->currentLevel.levelData) {
		if (tile->visible) {
			Vec2 tilePosition = tile->position;
			tilePosition.x -= tile->dimensions.x / 2.0f;
			tilePosition.y += tile->dimensions.y / 2.0f; //+ve is down in SDL

			int tileScreenX = (int)(
				/*Arbitrary Adjustments*/	(float)(this->core->windowWidth - this->core->getTileScale()) / 2.0f +
				/*Positional Adjustments*/	(float)(this->core->getTileScale()) * (tilePosition.x - playerPosition.x) +
				/*Trail Offset*/			trailOffset
				);
			int tileScreenY = (int)(
				/*Arbitrary Adjustments*/	(float)(this->core->windowHeight - this->core->getHeightAdjustment()) +
				/*Positional Adjustments*/	(float)(this->core->getTileScale()) * (-tilePosition.y + playerPosition.y)
				);

			renderTile(tileScreenX, tileScreenY, tile);
			if (renderDebug) {
				SDL_SetRenderDrawColor(this->gameRenderer, 0, 0, 255, 255);
				SDL_RenderDrawLine(this->gameRenderer, tileScreenX, tileScreenY, tileScreenX + this->core->getTileScale(), tileScreenY + this->core->getTileScale());
				SDL_RenderDrawLine(this->gameRenderer, tileScreenX, tileScreenY + this->core->getTileScale(), tileScreenX + this->core->getTileScale(), tileScreenY);
			}
		}
	}

		//PLAYER AMBIENCE
	SDL_SetRenderTarget(this->gameRenderer, this->gameLightMap);
	SDL_SetTextureBlendMode(this->baseTextures.effect_playerAmbience->surface, SDL_BLENDMODE_ADD);
	SDL_RenderCopy(
		this->gameRenderer,
		this->baseTextures.effect_playerAmbience->surface,
		NULL,
		&GetRect(
			this->core->getTileScale() * 4,
			this->core->getTileScale() * 4,
			(this->core->windowWidth - this->core->getTileScale() * 4) / 2 + (int)trailOffset,
			(this->core->windowHeight - this->core->getTileScale() * 3 - this->core->getHeightAdjustment())
		)
	);


	//Render Foreground

	//Render Gamma Light
	SDL_SetRenderTarget(this->gameRenderer, this->gameLightMap);
	SDL_SetRenderDrawBlendMode(this->gameRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(this->gameRenderer, 255, 255, 255, this->lightingGamma);
	SDL_RenderFillRect(this->gameRenderer, NULL);

	//Apply Lighting
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	SDL_SetTextureBlendMode(this->gameLightMap, SDL_BLENDMODE_MOD);
	SDL_RenderCopy(this->gameRenderer, this->gameLightMap, NULL, NULL);

	//Render Player
	this->renderCharacter();
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	int newPlayerTextureScale = 500 * (this->core->getTileScale() / 20);
	SDL_RenderCopy(
		this->gameRenderer,
		this->gamePlayerMap,
		NULL,
		&GetRect(
			newPlayerTextureScale,
			newPlayerTextureScale,
			(this->core->windowWidth - newPlayerTextureScale) / 2 + (int)trailOffset,
			(this->core->windowHeight - newPlayerTextureScale - this->core->getHeightAdjustment())
		)
	);

	//Render GUI
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	if (renderDebug) {
		SDL_SetRenderDrawColor(this->gameRenderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(this->gameRenderer, this->core->windowWidth / 2, 0, this->core->windowWidth / 2, this->core->windowHeight);
		SDL_RenderDrawLine(this->gameRenderer, 0, this->core->windowHeight / 2, this->core->windowWidth, this->core->windowHeight / 2);
	}
	if (this->gameData->currentGameMode == GameMode::EDITOR) {
		this->cogwheelRotation += deltaTime;
		if (this->cogwheelRotation > 1.0f) {
			this->cogwheelRotation = 0.0f;
		}
		SDL_RenderCopyEx(
			this->gameRenderer,
			this->baseTextures.gui_cogwheel->surface,
			NULL,
			&GetRect(core->getTileScale(), core->getTileScale(), 0, 0),
			360.0 * (double)this->cogwheelRotation,
			NULL,
			SDL_RendererFlip::SDL_FLIP_NONE
		);
	}
	else {
		this->cogwheelRotation = 0.0f;
	}

	//Present Render To Screen
	SDL_SetRenderTarget(this->gameRenderer, NULL);
	SDL_RenderCopy(this->gameRenderer, this->gameSurfaceMap, NULL, NULL);
	SDL_RenderPresent(this->gameRenderer);

	return 0;
}

void Renderer::renderTile(float x, float y, GameTile* tile){
	GameTexture* tileTexture = this->baseTextures.fromGameTileIDEnum(tile->tileID);

	if (tileTexture != nullptr && tileTexture->surface != nullptr) {
		SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
		SDL_RenderCopy(
			gameRenderer,
			tileTexture->surface,
			NULL,
			&GetRect(this->core->getTileScale(), this->core->getTileScale(), x, y)
		);
		if (tileTexture->bump != nullptr) {
			SDL_SetRenderTarget(this->gameRenderer, this->gameLightMap);
			SDL_RenderCopy(
				gameRenderer,
				tileTexture->bump,
				NULL,
				&GetRect(this->core->getTileScale(), this->core->getTileScale(), x, y)
			);
		}
	}

}

void Renderer::generateGradient(SDL_Texture* tex, SDL_Colour a, SDL_Colour b, bool dir) {
	//dir ? vertical : horizontal
	
	SDL_Texture* beforeTex = SDL_GetRenderTarget(this->gameRenderer);
	SDL_SetRenderTarget(this->gameRenderer, tex);

	int texW = 0; int texH = 0;
	SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);

	for (int i = 0; i < texH; i++) {

		float percent = (float)i / (float)texH;
		SDL_SetRenderDrawColor(this->gameRenderer,
			Lerp(a.r, b.r, percent),
			Lerp(a.g, b.g, percent),
			Lerp(a.b, b.b, percent),
			Lerp(a.a, b.a, percent)
		);
		SDL_RenderDrawLine(this->gameRenderer, 0, i, texW, i);
	}

	SDL_SetRenderTarget(this->gameRenderer, beforeTex);
	
}

float Renderer::getTrailAmount() {
	return (float)this->core->getTileScale() / 2.0f;
}

Renderer::Renderer(GameData* gameData, Core* core) : gameData(gameData), core(core) {
	//Set values integral to the renderer.
	this->trailOffset = 0.0f;
	this->lightingBrightness = 255;
	this->lightingGamma = 0;
	this->cogwheelRotation = 0.0f;
}

GameTexture* BaseTextures::fromGameTileIDEnum(GameTileIDEnum id){

	switch (id) {
	case AIR:
		return nullptr;
	case BRICK:
		return this->brick;
	case GRASS:
		return this->tiles_grass;
	default:
		return nullptr;
	}
}
