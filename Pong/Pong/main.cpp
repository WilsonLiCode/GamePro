//Wilson Li Pong
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

class Entity
{
public:
	Entity()
		: x(0.0f), y(0.0f), rotation(0.0f), directionX(0.0f), directionY(0.0f){}
	void Draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);

		GLfloat quad[] = { 0.0f, height, 0.0f, 0.0f, width, 0.0f, width, height };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
	void SetSize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	float x;
	float y;
	float rotation;

	int textureID;

	float width;
	float height;

	float directionX;
	float directionY;
};

Entity ball;
Entity paddle1;
Entity paddle2;

GLuint loadTexture(const char *image_path)
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

void setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool processEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
		{
			return false;
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		paddle1.directionY = 0.0;
		paddle2.directionY = 0.0;

		if (keys[SDL_SCANCODE_UP])
		{
			paddle1.directionY = 1.0;
		}
		else if (keys[SDL_SCANCODE_DOWN])
		{
			paddle1.directionY = -1.0;
		}
		if (keys[SDL_SCANCODE_W])
		{
			paddle2.directionY = 1.0;
		}
		else if (keys[SDL_SCANCODE_S])
		{
			paddle2.directionY = -1.0;
		}
	}
	return true;
}

void render(double& state)
{
	//Change screen color for about a second when a player wins
	if (state > 0)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		state -= 0.0003;
		if (state < 0)
			state = 0.0;
	}
	else if (state < 0)
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		state += 0.0003;
		if (state > 0)
			state = 0.0;
	}
	else
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	ball.Draw();
	paddle1.Draw();
	paddle2.Draw();

	SDL_GL_SwapWindow(displayWindow);
}

void update(float elapsed)
{
	//Control paddles
	paddle1.y += paddle1.directionY * elapsed;
	paddle2.y += paddle2.directionY * elapsed;
	if (paddle1.y > 0.7f)
		paddle1.y = 0.7f;
	else if(paddle1.y < -1.0f)
		paddle1.y = -1.0f;
	if (paddle2.y > 0.7f)
		paddle2.y = 0.7f;
	else if (paddle2.y < -1.0f)
		paddle2.y = -1.0f;

	//Control ball
	ball.x += ball.directionX * elapsed;
	ball.y += ball.directionY * elapsed;
}

void check(double& state)
{
	float ballX = ball.x + ball.width; //right side of ball
	float pad1X = paddle1.x - paddle1.height; //left side of paddle 1
	float ballY = ball.y + ball.height; //top of ball
	float pad1Y = paddle1.y + paddle1.width; //top of paddle 1
	float pad2Y = paddle2.y + paddle2.width; //top of paddle 2

	//Check right wall
	if (ballX >= pad1X && ballY > paddle1.y && ball.y < pad1Y)
	{
		ball.x -= 0.01f;
		ball.directionX *= -1.0f;
		if (paddle1.directionY > 0.0f)
			ball.directionY = 1.0f;
		else if (paddle1.directionY < 0.0f)
			ball.directionY = -1.0f;
	}
	else if (ballX >= pad1X && (ballY <= paddle1.y || ball.y >= pad1Y))
	{
		ball.x = 0.0f;
		ball.y = 0.0f;
		ball.directionY = 0.0f;
		state = 1;
		return;
	}

	//Check left wall
	if (ball.x <= paddle2.x && ballY > paddle2.y && ball.y < pad2Y)
	{
		ball.x += 0.01f;
		ball.directionX *= -1.0f;
		if (paddle2.directionY > 0.0f)
			ball.directionY = 1.0f;
		else if (paddle2.directionY < 0.0f)
			ball.directionY = -1.0f;
	}
	else if (ball.x <= paddle2.x && (ballY <= paddle2.y || ball.y >= pad2Y))
	{
		ball.x = 0.0f;
		ball.y = 0.0f;
		ball.directionY = 0.0f;
		state = -1;
		return;
	}

	//Check top and bottom walls
	if (ballY > 1.0f)
	{
		ball.y -= 0.01f;
		ball.directionY *= -1.0f;
	}
	else if (ball.y < -1.0f)
	{
		ball.y += 0.01f;
		ball.directionY *= -1.0f;
	}
}

int main(int argc, char *argv[])
{
	setup();

	float lastFrameTicks = 0.0f;

	double state = 0.0;

	//Setup objects
	paddle1.textureID = loadTexture("paddleRed.png");
	paddle1.SetSize(0.3f, 0.08f);
	paddle1.rotation = 90.0f;
	paddle1.x = 1.2f;
	paddle1.y = -0.11f;
	paddle2.textureID = loadTexture("paddleBlu.png");
	paddle2.SetSize(0.3f, 0.08f);
	paddle2.rotation = 90.0f;
	paddle2.x = -1.2f;
	paddle2.y = -0.11f;
	ball.textureID = loadTexture("ballGrey.png");
	ball.SetSize(0.08f, 0.08f);
	ball.directionX = 1.0f;

	while (processEvents()) 
	{
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		update(elapsed);
		check(state);
		render(state);
	}

	SDL_Quit();
	return 0;
}