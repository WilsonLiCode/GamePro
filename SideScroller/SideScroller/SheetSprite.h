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
	SheetSprite(GLuint textureID, int index, int spriteCountX, int spriteCountY);

	void Draw();
	void setAnimated(bool val, float fps, vector<float> frames);

	GLuint textureID;

	int index;
	int spriteCountX;
	int spriteCountY;

	bool animated;
	float framesPerSecond;
	vector<float> frames;
	int animationIndex;
	float animationElapsed;

	bool visible;
};