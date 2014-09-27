//Wilson Li GameApp Implementation
#include "GameApp.h"

enum GameState { STATE_TITLE, STATE_GAME, STATE_GAMEOVER };

GameApp::GameApp() 
{
	Init();
	done = false;
	lastFrameTicks = 0.0f;
	state = STATE_TITLE;
	animationElapsed = 0.0f;
	lives = 2;
	score = 0;
	bulletIndex = 0;
	bulletTimer = 0;
	shipBullet.maxTime = 0.7f;
	shipBullet.speed = 2.0f;

	fontText = loadTexture("pixel_font.png");
	spriteSheetText = loadTexture("spriteSheet.png");
	
	float xPos = -0.5f;
	for (int i = 0; i < 11; i++)
	{
		aliens.push_back(new SheetSprite(spriteSheetText, 0.4f, 32.0f / 256.0f, 102.0f / 128.0f, 24.0f / 256.0f, 16.0f / 128.0f, xPos, 0.7f, 110.0f / 256.0f, 86.0f / 128.0f));
		aliens.push_back(new SheetSprite(spriteSheetText, 0.4f, 58.0f / 256.0f, 102.0f / 128.0f, 24.0f / 256.0f, 16.0f / 128.0f, xPos, 0.55f, 110.0f / 256.0f, 104.0f / 128.0f));
		aliens.push_back(new SheetSprite(spriteSheetText, 0.4f, 58.0f / 256.0f, 102.0f / 128.0f, 24.0f / 256.0f, 16.0f / 128.0f, xPos, 0.4f, 110.0f / 256.0f, 104.0f / 128.0f));
		aliens.push_back(new SheetSprite(spriteSheetText, 0.4f, 84.0f / 256.0f, 102.0f / 128.0f, 24.0f / 256.0f, 16.0f / 128.0f, xPos, 0.25f, 100.0f / 256.0f, 68.0f / 128.0f));
		aliens.push_back(new SheetSprite(spriteSheetText, 0.4f, 84.0f / 256.0f, 102.0f / 128.0f, 24.0f / 256.0f, 16.0f / 128.0f, xPos, 0.1f, 100.0f / 256.0f, 68.0f / 128.0f));
		xPos += 0.1f;
	}
	ship = SheetSprite(spriteSheetText, 0.4f, 0.0f / 256.0f, 102.0f / 128.0f, 30.0f / 256.0f, 16.0f / 128.0f, 0.0f, -0.7f);
}

void GameApp::Init() 
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

GameApp::~GameApp() 
{
	SDL_Quit();
}

void GameApp::Render() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state)
	{
	case STATE_TITLE:
		glLoadIdentity();
		glTranslatef(-0.65f, 0.7f, 0.0f);
		DrawText(fontText, "SPACE INVADERS", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.5f, -0.5f, 0.0f);
		DrawText(fontText, "Press SPACE to start", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case STATE_GAME:
		glLoadIdentity();
		glTranslatef(-0.9f, -0.9f, 0.0f);
		DrawText(fontText, string("Lives: ") + to_string(lives), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.9f, 0.9f, 0.0f);
		DrawText(fontText, string("Score: ") + to_string(score), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		for (int i = 0; i < aliens.size(); i++)
			aliens[i]->Draw();
		ship.Draw();
		shipBullet.Draw();
		for (int i = 0; i < MAX_BULLETS; i++)
			alienBullets[i].Draw();
		break;
	case STATE_GAMEOVER:
		glLoadIdentity();
		glTranslatef(-0.4f, 0.5f, 0.0f);
		DrawText(fontText, "GAME OVER", 0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.25f, 0.0f, 0.0f);
		DrawText(fontText, string("Score: ") + to_string(score), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.5f, -0.5f, 0.0f);
		DrawText(fontText, "Press SPACE to restart", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}

void GameApp::ProcessEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
		{
			done = true;
		}
		else if (state == 1 && event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !shipBullet.visible)
				shipBullet.Reset(ship.x, ship.y + ship.height / 2.0f);
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL);

		ship.directionX = 0.0f;

		switch (state)
		{
		case 0:
			if (keys[SDL_SCANCODE_SPACE])
			{
				state = 1;
			}
			break;
		case 1:
			if (keys[SDL_SCANCODE_LEFT])
			{
				ship.directionX = -1.0f;
			}
			else if (keys[SDL_SCANCODE_RIGHT])
			{
				ship.directionX = 1.0f;
			}
			break;
		case 2:
			if (keys[SDL_SCANCODE_SPACE])
			{
				for (int i = 0; i < aliens.size(); i++)
					aliens[i]->visible = true;
				ship.x = 0.0f;
				ship.visible = true;
				score = 0;
				lives = 2;
				state = 1;
			}
			break;
		}
	}
}

void GameApp::Update(float elapsed)
{
	if (state != STATE_GAME)
		return;

	//update aliens
	animationElapsed += elapsed;
	if (animationElapsed > 1.0 / 3.0f)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			aliens[i]->curIndex++;
			animationElapsed = 0.0;
			if (aliens[i]->curIndex > 2)
				aliens[i]->curIndex = 0;
			aliens[i]->x += aliens[i]->directionX * 0.05f;
		}
	}
	if (aliens[aliens.size() - 1]->x > 0.8)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			aliens[i]->directionX = -1.0f;
		}
	}
	else if (aliens[0]->x < -0.8)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			aliens[i]->directionX = 1.0f;
		}
	}

	//update ship
	ship.x += ship.directionX * elapsed;
	if (ship.x < -0.9)
		ship.x = -0.9f;
	else if (ship.x > 0.9)
		ship.x = 0.9f;
	if (ship.visible)
	{
		shipTimer = 0;
	}
	else
	{
		shipTimer += 1;
		if (shipTimer > 1000)
		{
			ship.visible = true;
			ship.x = 0.0f;
		}
	}

	//update bullets
	shipBullet.Update(elapsed);

	for (int i = 0; i < MAX_BULLETS; i++)
		alienBullets[i].Update(elapsed);
	bulletTimer++;
	if (bulletTimer > 1000)
	{
		int randIndex = rand() % aliens.size();
		while (aliens[randIndex]->visible == false)
			randIndex = rand() % aliens.size();
		if (!alienBullets[bulletIndex].visible)
		{
			alienBullets[bulletIndex].Reset(aliens[randIndex]->x, aliens[randIndex]->y);
			bulletIndex++;
		}
		if (bulletIndex > MAX_BULLETS - 1)
			bulletIndex = 0;
		bulletTimer = 0;
	}
}

void GameApp::Check()
{
	if (state != STATE_GAME)
		return;

	state = STATE_GAMEOVER;
	for (int i = 0; i < aliens.size(); i++)
		if (aliens[i]->visible)
			state = STATE_GAME;
	if (lives < 0)
		state = STATE_GAMEOVER;

	//Check player bullets
	if (shipBullet.visible)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			float alienLeft = aliens[i]->x - aliens[i]->width / 2.0f;
			float alienRight = aliens[i]->x + aliens[i]->width / 2.0f;
			float alienBottom = aliens[i]->y - aliens[i]->height / 2.0f;
			float alienTop = aliens[i]->y + aliens[i]->height / 2.0f;
			if (aliens[i]->visible && shipBullet.x >= alienLeft && shipBullet.x <= alienRight && shipBullet.y + 3.0f*shipBullet.size >= alienBottom && shipBullet.y + 3.0f*shipBullet.size < alienTop)
			{
				shipBullet.visible = false;
				aliens[i]->visible = false;
				score += 100;
				break;
			}
		}
	}

	//Check enemy bullets
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		float shipLeft = ship.x - ship.width / 2.0f;
		float shipRight = ship.x + ship.width / 2.0f;
		float shipTop = ship.y + ship.height / 2.0f;
		float shipBottom = ship.y - ship.height / 2.0f;
		if (ship.visible && alienBullets[i].visible && alienBullets[i].x >= shipLeft && alienBullets[i].x <= shipRight && alienBullets[i].y - 3.0f*alienBullets[i].size <= shipTop && alienBullets[i].y - 3.0f*alienBullets[i].size > shipBottom)
		{
			alienBullets[i].visible = false;
			ship.visible = false;
			lives -= 1;
			break;
		}
	}
}

bool GameApp::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ProcessEvents();
	Update(elapsed);
	Check();
	Render();
	return done;
}