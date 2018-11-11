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

void Renderer::renderCharacter() {
	//characterHandler->renderAtPos(gameRenderer, windowWidth / 2, heightAdjustment - 180);
	CharacterHandler* characterHandler = this->gameData->characterHandler;
	SDL_RendererFlip flipState = characterHandler->direction ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	int xPos = this->windowWidth / 2;
	int yPos = this->heightAdjustment - /*size of character x 3*/ (this->tileScale * 3);

	/*
	To fix our issue, we need to have a standard.
	Therefore, the standard is that all textures must align to the left.

	Hence, we now need to know:
	The centre point of each limb texture.
	(For the head, this would be the 4th px along.)

	Where this "centre point" is relative to the
	position of the character.
	(For instance, the head is the 4th px along relative to the texture
	BUT we should make sure this 4th px along is the centre of the character..)
	*/

	float originalToTileScale = (float)tileScale / 20.0f; //tileScale / sizeOfTex

	//BACK ARM (Horizontal centre point: 2nd px)
	SDL_RenderCopyEx(
		this->gameRenderer,
		this->baseTextures.player_arm->surface,
		&GetRect(
			20,
			20,
			20 * (int)(8.5 * characterHandler->arm_back_tension),
			0
		),
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 2.0f : 2.0f) * originalToTileScale),
			yPos + (int)(22.0f * originalToTileScale)
		),
		characterHandler->arm_back_rotation,
		&GetPoint(
		(int)((characterHandler->direction ? 20.0f - 2.0f : 2.0f) * originalToTileScale), 0
		),
		flipState
	);

	//BACK LEG (Horizontal centre point: 13th px)
	SDL_RenderCopyEx(
		this->gameRenderer,
		this->baseTextures.player_leg->surface,
		&GetRect(
			20,
			20,
			20 * (int)(4.5f * characterHandler->leg_back_tension),
			0
		),
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 13.0f : 13.0f) * originalToTileScale),
			yPos + (int)(40.0f * originalToTileScale)),
		characterHandler->leg_back_rotation,
		&GetPoint(
		(int)((characterHandler->direction ? 20.0f - 13.0f : 13.0f) * originalToTileScale), 0
		),
		flipState
	);

	//HEAD (Horizontal centre point: 4th px)
	SDL_RenderCopyEx(
		this->gameRenderer,
		this->baseTextures.player_head->surface,
		NULL,
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 4.0f : 4.0f) * originalToTileScale),
			yPos),
		0.0,
		NULL,
		flipState
	);

	//FRONT LEG (Horizontal centre point: 13th px)
	SDL_RenderCopyEx(
		this->gameRenderer,
		this->baseTextures.player_leg->surface,
		&GetRect(
			20,
			20,
			20 * (int)(4.5f * characterHandler->leg_frnt_tension),
			0
		),
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 13.0f : 13.0f) * originalToTileScale),
			yPos + (int)(40.0f * originalToTileScale)),
		characterHandler->leg_frnt_rotation,
		&GetPoint(
		(int)((characterHandler->direction ? 20.0f - 13.0f : 13.0f) * originalToTileScale), 0
		),
		flipState
	);

	//BODY (Horizontal centre point: 3rd px)
	SDL_RenderCopyEx(
		this->gameRenderer,
		this->baseTextures.player_body->surface,
		NULL,
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 3.0f : 3.0f) * originalToTileScale),
			yPos + (int)(20.0f * originalToTileScale)),
		0.0,
		NULL,
		flipState
	);

	//FRONT ARM (Horizontal centre point: 2nd px)
	SDL_RenderCopyEx(
		gameRenderer,
		this->baseTextures.player_arm->surface,
		&GetRect(
			20,
			20,
			20 * (int)(8.5f * characterHandler->arm_frnt_tension),
			0
		),
		&GetRect(
			this->tileScale,
			this->tileScale,
			xPos - (int)((characterHandler->direction ? 20.0f - 2.0f : 2.0f) * originalToTileScale),
			yPos + (int)((20.0f + 2.0f) * originalToTileScale)
		),
		characterHandler->arm_frnt_rotation,
		&GetPoint(
		(int)((characterHandler->direction ? 20.0f - 2.0f : 2.0f) * originalToTileScale), 0
		),
		flipState
	);

	if (renderDebug) {
		SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(gameRenderer, xPos, yPos, xPos, yPos + (tileScale * 3));

		//AABB colBox = AABB(yPos + (tileScale * 3), yPos, xPos - tileScale / 2, xPos + tileScale / 2);

		float left = xPos - (tileScale * characterHandler->dimensions.x) / 2.0f;
		float right = xPos + (tileScale * characterHandler->dimensions.x) / 2.0f;
		float top = yPos + (tileScale * characterHandler->dimensions.y);
		float bottom = yPos;

		SDL_SetRenderDrawColor(gameRenderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(gameRenderer, left, top, right, top);
		SDL_RenderDrawLine(gameRenderer, right, top, right, bottom);
		SDL_RenderDrawLine(gameRenderer, left, bottom, right, bottom);
		SDL_RenderDrawLine(gameRenderer, left, top, left, bottom);
	}
}

int Renderer::init() {
	//Init SDL Image
	int imgInitFlags = IMG_INIT_PNG;
	int imgInitState = IMG_Init(imgInitFlags);
	if ((imgInitState & imgInitFlags) != imgInitFlags) {
		return -1;
	}

	//Create Window & Renderer
	gameWindow = SDL_CreateWindow("Sidescroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (gameWindow == nullptr || gameRenderer == nullptr) {
		return -1;
	}

	//Load Resources
	baseTextures.brick = GameTexture::LoadImage(gameRenderer, "bricks");
	baseTextures.player_arm = GameTexture::LoadImage(gameRenderer, "player/arm");
	baseTextures.player_body = GameTexture::LoadImage(gameRenderer, "player/body");
	baseTextures.player_head = GameTexture::LoadImage(gameRenderer, "player/head");
	baseTextures.player_leg = GameTexture::LoadImage(gameRenderer, "player/leg");

	//Create Lighting Texture
	gameSurfaceMap = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
	gameLightMap = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

	return 0;
}

int Renderer::update(float deltaTime) {
	if (this->gameData->inputHandler->inputActivity.debug && renderDebug_oneTime) {
		renderDebug = !renderDebug;
		renderDebug_oneTime = false;
	}

	if (!this->gameData->inputHandler->inputActivity.debug) {
		renderDebug_oneTime = true;
	}

	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	SDL_SetRenderDrawColor(this->gameRenderer, 0, 0, 0, 255);
	SDL_RenderClear(this->gameRenderer);

	/*SDL_SetRenderTarget(this->gameRenderer, this->gameLightMap);
	SDL_SetRenderDrawColor(this->gameRenderer, 255, 255, 255, 255);
	SDL_RenderClear(this->gameRenderer);*/

	//Render Sky

	//Render Background
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	for (GameTile* tile : this->gameData->currentLevel.levelData) {

		b2Vec2 tilePosition = tile->body->GetPosition();
		tilePosition.x -= tile->dimensions.x / 2.0f;
		tilePosition.y += tile->dimensions.y / 2.0f; //+ve is down in SDL
		b2Vec2 playerPosition = this->gameData->characterHandler->body->GetPosition();
		playerPosition.x -= this->gameData->characterHandler->dimensions.x / 2.0f;
		playerPosition.y -= this->gameData->characterHandler->dimensions.y / 2.0f; //-ve is down in Box2D

		int tileScreenX = (int)(
			/*Arbitrary Adjustments*/	(float)(windowWidth - tileScale)/ 2.0f +
			/*Positional Adjustments*/	(float)(tileScale) * (tilePosition.x - playerPosition.x)
			);
		int tileScreenY = (int)(
			/*Arbitrary Adjustments*/	(float)heightAdjustment +
			/*Positional Adjustments*/	(float)(tileScale) * (-tilePosition.y + playerPosition.y)
			);
		
		RenderTile(tileScreenX, tileScreenY, tile);
	}

	//Render Player
	SDL_SetRenderTarget(this->gameRenderer, this->gameSurfaceMap);
	this->renderCharacter();

	//Render Foreground

	//Render GUI

	//Present Render To Screen
	SDL_SetRenderTarget(this->gameRenderer, NULL);
	SDL_RenderCopy(this->gameRenderer, this->gameSurfaceMap, NULL, NULL);
	SDL_RenderPresent(this->gameRenderer);

	return 0;
}

void Renderer::RenderTile(float x, float y, GameTile* tile){
	GameTexture* tileTexture = this->baseTextures.fromGameTileIDEnum(tile->tileID);
	if (tileTexture != nullptr) {
		SDL_RenderCopy(
			gameRenderer,
			tileTexture->surface,
			NULL,
			&GetRect(tileScale, tileScale, x, y)
		);
	}

}

Renderer::Renderer(GameData* gameData) : gameData(gameData) {
}

GameTexture* BaseTextures::fromGameTileIDEnum(GameTileIDEnum id){

	switch (id) {
	case AIR:
		return nullptr;
	case BRICK:
		return this->brick;
	default:
		return nullptr;
	}
}
