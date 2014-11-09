//Wilson Li OtherFunctions Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Matrix.h"

#define PI 3.14159265359f

using namespace std;

GLuint loadTexture(const char *image_path, GLenum format = GL_RGBA);

void DrawText(int textureID, string text, float size, float spacing, float r, float g, float b, float a);
void DrawSprite(GLint texture, float x, float y, float scale, bool additive);

float lerp(float v0, float v1, float t);

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
float easeIn(float from, float to, float time);
float easeOut(float from, float to, float time);
float easeInOut(float from, float to, float time);
float easeOutElastic(float from, float to, float time);