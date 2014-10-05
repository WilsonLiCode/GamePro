//Wilson Li Platformer
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "GameApp.h"

int main(int argc, char *argv[])
{
	GameApp app;
	while (!app.UpdateAndRender()) {}
	return 0;
}