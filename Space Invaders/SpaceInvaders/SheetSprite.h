//Wilson Li SheetSprite Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite
{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float scale, float u, float v, float width, float height, float x, float y, float u1=0.0f, float v1=0.0f);

	void Draw();

	float scale;
	GLuint textureID;
	float u;
	float v;
	float u1;
	float v1;
	float width;
	float height;
	float x;
	float y;
	float directionX;
	int curIndex;
	bool visible;
};