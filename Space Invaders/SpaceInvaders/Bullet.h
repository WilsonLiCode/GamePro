//Wilson Li Bullet Header
#pragma once

#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define PI 3.14159265359

class Bullet
{
public:
	Bullet();
	Bullet(float x, float y, float maxTime);
	void Update(float elapsed);
	void Draw();
	void Reset(float newX, float newY);

	float x;
	float y;

	float size;
	float angle;
	float speed;

	bool visible;
	float timeAlive;
	float maxTime;
};