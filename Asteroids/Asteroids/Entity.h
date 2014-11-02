//Wilson Li Entity Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SheetSprite.h"
#include "Matrix.h"
#include "OtherFunctions.h"

#define FIXED_TIMESTEP 0.0166666f
#define PI 3.14159265359f

class Entity
{
public:
	Entity();

	void Draw();
	void BuildMatrix();
	void FixedUpdate();
	float getHeight();
	float getWidth();

	SheetSprite sprite;

	float x;
	float y;
	float rotation;
	float scaleX;
	float scaleY;

	float velocity;
	float rotationVelocity;
	float acceleration;
	float rotationAcceleration;
	float friction;

	Matrix matrix;

	bool collided;
};