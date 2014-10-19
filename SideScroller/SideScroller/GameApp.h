//Wilson Li GameApp Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "OtherFunctions.h"
#include "SheetSprite.h"
#include "Entity.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define MAX_TIMESTEPS 6
#define NUM_STARS 7
#define LEVEL_HEIGHT 32
#define LEVEL_WIDTH 128
#define TILE_SIZE 0.05f
#define SPRITE_COUNT_X 64
#define SPRITE_COUNT_Y 32
#define WORLD_OFFSET_X 0.0f
#define WORLD_OFFSET_Y 0.0f

using namespace std;

class GameApp 
{
public:
	GameApp();
	~GameApp();
	void Init();
	bool UpdateAndRender();
	void Render();
	void ProcessEvents();
	void FixedUpdate();
	void Update(float elapsed);
	void Collision();

	void BuildLevel();
	bool readHeader(ifstream &stream);
	bool readLayerData(ifstream &stream);
	bool readEntityData(ifstream &stream);

	void placeEntity(string type, float placeX, float placeY);
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
	bool isSolid(unsigned int tile);
	float checkPointForGridCollisionsY(float x, float y);
	float checkPointForGridCollisionsX(float x, float y);
	void doLevelCollisionY(Entity* entity);
	void doLevelCollisionX(Entity* entity);

private:
	const Uint8 *keys;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	int state;
	bool dead;

	GLuint fontText;
	GLuint spriteSheet;

	Mix_Music* music;
	Mix_Chunk* jumpSound;
	Mix_Chunk* starSound;

	int mapWidth;
	int mapHeight;

	float gravityX;
	float gravityY;

	unsigned int** levelData;

	Entity player;
	vector<Entity*> entities;
	int starsCollected;
	vector<Entity*> stars;
};