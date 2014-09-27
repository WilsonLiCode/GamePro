//Wilson Li GameApp Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "OtherFunctions.h"
#include "SheetSprite.h"
#include "Bullet.h"

#define MAX_BULLETS 5

class GameApp 
{
public:
	GameApp();
	~GameApp();
	void Init();
	bool UpdateAndRender();
	void Render();
	void ProcessEvents();
	void Update(float elapsed);
	void Check();

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	int state;
	float animationElapsed;
	int lives;
	int score;

	GLuint fontText;
	GLuint spriteSheetText;
	vector<SheetSprite*> aliens;
	SheetSprite ship;
	Bullet shipBullet;
	int shipTimer;
	int bulletIndex;
	int bulletTimer;
	Bullet alienBullets[MAX_BULLETS];
};