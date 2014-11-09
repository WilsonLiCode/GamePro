//Wilson Li SheetSprite Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

using namespace std;

class SheetSprite
{
public:
	SheetSprite();
	SheetSprite(GLuint textureID, float scale, float u, float v, float width, float height);

	void Draw();

	GLuint textureID;

	float scale;
	float u;
	float v;
	float width;
	float height;

	bool visible;
};