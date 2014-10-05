//Wilson Li GameApp Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "OtherFunctions.h"
#include "SheetSprite.h"
#include "Entity.h"

#define MAX_TIMESTEPS 6
#define NUM_COINS 11

class GameApp 
{
public:
	GameApp();
	~GameApp();
	void Init();
	bool UpdateAndRender();
	void Render(float elapsed);
	void ProcessEvents();
	void FixedUpdate();
	void Update(float elapsed);
	void Collision();
	void BuildLevel();

private:
	const Uint8 *keys;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint fontText;
	GLuint spriteSheet;

	float gravityX;
	float gravityY;

	Entity player;
	vector<Entity*> entities;
	Entity blocks[248];
	Entity coins[NUM_COINS];
	int coinsCollected;
};