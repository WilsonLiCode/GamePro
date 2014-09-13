//Wilson Li Scene
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) 
{
	SDL_Surface *surface = IMG_Load(image_path);
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float scale) 
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(scale, 1.0, 1.0);

	GLfloat quad[] = {0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	SDL_Event event;

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	GLuint cannon = LoadTexture("laserRight.png");
	GLuint block = LoadTexture("metal.png");
	GLuint beam = LoadTexture("laserRedHorizontal.png");
	GLuint burst = LoadTexture("laserRedBurst.png");

	float lastFrameTicks = 0.0f;
	float beamScale = 0.0f;

	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				done = true;
			}
		}

		//Keep track of time
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks; //elapsed = # of secs elapsed since last frame
		lastFrameTicks = ticks;

		//OpenGL reset color of screen
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(0.0f, 0.7f, 0.0f);
		GLfloat triangle[] = { 0.0f, 0.2f, -0.5f, -0.5f, 0.5f, -0.5f };
		GLfloat triangleColors[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, triangleColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableClientState(GL_COLOR_ARRAY);

		DrawSprite(cannon, -1.2f, -0.5f, 1.0f);
		DrawSprite(block, 0.8f, -0.5f, 1.0f);

		if (beamScale < 3)
		{
			DrawSprite(beam, -0.7f, -0.5f, beamScale);
			beamScale += elapsed;
		}
		else
		{
			DrawSprite(beam, -0.7f, -0.5f, 3.0f);
			DrawSprite(burst, 0.55f, -0.5f, 1.0f);
			beamScale += elapsed;
		}

		if (beamScale > 3.5)
			beamScale = 0.0f;

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}