//Wilson Li OtherFunctions Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>

using namespace std;

GLuint loadTexture(const char *image_path);

void DrawText(int textureID, string text, float size, float spacing, float r, float g, float b, float a);