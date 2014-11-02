//Wilson Li Bullet Header
#pragma once

#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"

#define FIXED_TIMESTEP 0.0166666f
#define PI 3.14159265359f

class Bullet
{
public:
	Bullet();
	Bullet(float x, float y, float maxTime);
	void Update();
	void Draw();
	void Reset(float newX, float newY);
	void BuildMatrix();

	Matrix matrix;

	float x;
	float y;

	float size;
	float angle;
	float velocity;

	bool visible;
	float timeAlive;
	float maxTime;
};