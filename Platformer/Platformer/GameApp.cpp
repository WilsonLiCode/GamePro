//Wilson Li GameApp Implementation
#include "GameApp.h"

enum GameState { STATE_TITLE, STATE_GAME, STATE_GAMEOVER };

GameApp::GameApp() 
{
	Init();
	keys = SDL_GetKeyboardState(NULL);
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	fontText = loadTexture("pixel_font.png");
	spriteSheet = loadTexture("sprites.png");
	gravityX = 0.0f;
	gravityY = -0.1f;
	coinsCollected = 0;

	BuildLevel();

	//Build player
	player.sprite = SheetSprite(spriteSheet, 0.2f, 19.0f / 70.0f, 0.0f / 70.0f, 16.0f / 70.0f, 23.0f / 70.0f);
	player.flip = true;
	player.x = -0.8f;
	player.y = -0.5f;
	entities.push_back(&player);
}

void GameApp::Init() 
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void GameApp::BuildLevel()
{
	//Build Blocks
	int blockIndex = 0;
	float curX = -1.0f;
	float curY = -0.7f;
	for (int i = 0; i < 17; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curX += (8.0f / 70.0f);
	}
	for (int i = 0; i < 14; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curY += (8.0f / 70.0f);
	}
	for (int i = 0; i < 17; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curX -= (8.0f / 70.0f);
	}
	for (int i = 0; i < 2; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curY -= (8.0f / 70.0f);
	}
	curY -= 2.0f*(8.0f / 70.0f);
	for (int i = 0; i < 8; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curY -= (8.0f / 70.0f);
	}
	curX += (8.0f / 70.0f);
	curY += 6.0f*(8.0f / 70.0f);
	for (int i = 0; i < 7; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curX += (8.0f / 70.0f);
	}
	curX += 8.0f*(8.0f / 70.0f);
	curY -= 4.0f*(8.0f / 70.0f);
	for (int i = 0; i < 7; i++)
	{
		blocks[blockIndex].sprite = SheetSprite(spriteSheet, 0.5f, 60.0f / 70.0f, 36.0f / 70.0f, 8.0f / 70.0f, 10.0f / 70.0f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].x = curX;
		blocks[blockIndex].y = curY;
		entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		curX -= (8.0f / 70.0f);
	}

	//Build Coins
	vector<float> framesU;
	vector<float> framesV;
	framesU.push_back(61.0f / 70.0f);
	framesU.push_back(60.0f / 70.0f);
	framesU.push_back(48.0f / 70.0f);
	framesU.push_back(48.0f / 70.0f);
	framesU.push_back(61.0f / 70.0f);
	framesV.push_back(15.0f / 70.0f);
	framesV.push_back(48.0f / 70.0f);
	framesV.push_back(39.0f / 70.0f);
	framesV.push_back(54.0f / 70.0f);
	framesV.push_back(0.0f / 70.0f);
	curX = -0.8f;
	int coinIndex = 0;
	for (int i = 0; i<3; i++)
	{
		coins[coinIndex].sprite = SheetSprite(spriteSheet, 0.2f, 61.0f / 70.0f, 15.0f / 70.0f, 10.0f / 70.0f, 13.0f / 70.0f);
		coins[coinIndex].sprite.setAnimated(true, 8.0f, framesU, framesV);
		coins[coinIndex].x = curX;
		coins[coinIndex].y = 0.4f;
		coins[coinIndex].isStatic = false;
		coins[coinIndex].enableGravity = false;
		coins[coinIndex].enableCollisions = false;
		entities.push_back(&coins[coinIndex]);
		coinIndex++;
		curX += 0.25f;
	}
	curX += 0.75f;
	for (int i = 0; i<3; i++)
	{
		coins[coinIndex].sprite = SheetSprite(spriteSheet, 0.2f, 61.0f / 70.0f, 15.0f / 70.0f, 10.0f / 70.0f, 13.0f / 70.0f);
		coins[coinIndex].sprite.setAnimated(true, 8.0f, framesU, framesV);
		coins[coinIndex].x = curX;
		coins[coinIndex].y = 0.0f;
		coins[coinIndex].isStatic = false;
		coins[coinIndex].enableGravity = false;
		coins[coinIndex].enableCollisions = false;
		entities.push_back(&coins[coinIndex]);
		coinIndex++;
		curX -= 0.25f;
	}
	curX -= 0.25f;
	for (int i = 0; i<5; i++)
	{
		coins[coinIndex].sprite = SheetSprite(spriteSheet, 0.2f, 61.0f / 70.0f, 15.0f / 70.0f, 10.0f / 70.0f, 13.0f / 70.0f);
		coins[coinIndex].sprite.setAnimated(true, 8.0f, framesU, framesV);
		coins[coinIndex].x = curX;
		coins[coinIndex].y = -0.5f;
		coins[coinIndex].isStatic = false;
		coins[coinIndex].enableGravity = false;
		coins[coinIndex].enableCollisions = false;
		entities.push_back(&coins[coinIndex]);
		coinIndex++;
		curX += 0.25f;
	}
}

GameApp::~GameApp() 
{
	SDL_Quit();
}

void GameApp::Render(float elapsed) 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < entities.size(); i++)
		entities[i]->Draw(elapsed);

	glLoadIdentity();
	glTranslatef(-0.9f, -0.9f, 0.0f);
	DrawText(fontText, string("Coins Collected: ") + to_string(coinsCollected), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

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

		player.accelerationX = 0.0f;
		player.accelerationY = 0.0f;

		if (keys[SDL_SCANCODE_RIGHT])
		{
			player.flip = true;
			player.accelerationX = 0.1f;
		}
		else if (keys[SDL_SCANCODE_LEFT])
		{
			player.flip = false;
			player.accelerationX = -0.1f;
		}
	}
}

void GameApp::FixedUpdate()
{
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->FixedUpdate();
		if (!entities[i]->isStatic && entities[i]->enableGravity)
		{
			entities[i]->velocityX += gravityX * FIXED_TIMESTEP;
			entities[i]->velocityY += gravityY * FIXED_TIMESTEP;
		}
		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;
	}
	if (player.x < -0.95 && player.y > 0.5)
	{
		player.x = -0.95f;
		player.y = -0.57f;
		for (int i = 0; i < NUM_COINS; i++)
			coins[i].sprite.visible = true;
	}
	else if (player.x < -0.95)
	{
		player.x = -0.95f;
		player.y = -0.57f;
	}
}

void GameApp::Collision()
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (!entities[i]->isStatic && entities[i]->sprite.visible)
		{
			//y axis
			entities[i]->y += entities[i]->velocityY * FIXED_TIMESTEP;
			for (int j = 0; j < entities.size(); j++)
			{
				if (j != i && entities[j]->sprite.visible)
				{
					if (entities[i]->collidesWith(entities[j]))
					{
						float yPenetration = fabs(entities[i]->y - entities[j]->y) - entities[j]->sprite.height / 1.1f;
						if (entities[i]->y > entities[j]->y)
						{
							entities[i]->collidedBottom = true;
							entities[i]->velocityY = 0.0f;
							if (entities[i]->enableCollisions)
								entities[i]->y += yPenetration + 0.001f;
						}
						else
						{
							entities[i]->collidedTop = true;
							entities[i]->velocityY = 0.0f;
							if (entities[i]->enableCollisions)
								entities[i]->y -= yPenetration + 0.001f;
						}
					}
				}
			}
			
			//x axis
			entities[i]->x += entities[i]->velocityX * FIXED_TIMESTEP;
			for (int j = 0; j < entities.size(); j++)
			{
				if (j != i && entities[j]->sprite.visible)
				{
					if (entities[i]->collidesWith(entities[j]))
					{
						float xPenetration = fabs(entities[i]->x - entities[j]->x) - entities[j]->sprite.width / 1.1f;
						if (entities[i]->x > entities[j]->x)
						{
							entities[i]->collidedLeft = true;
							entities[i]->velocityX = 0.0f;
							if (entities[i]->enableCollisions)
								entities[i]->x += xPenetration + 0.001f;
						}
						else
						{
							entities[i]->collidedRight = true;
							entities[i]->velocityX = 0.0f;
							if (entities[i]->enableCollisions)
								entities[i]->x -= xPenetration + 0.001f;
						}
					}
				}
			}
		}
	}

	//Check for hit coins
	for (int i = 0; i < NUM_COINS; i++)
	{
		if ((coins[i].collidedLeft || coins[i].collidedRight || coins[i].collidedTop || coins[i].collidedBottom) && coins[i].sprite.visible)
		{
			coins[i].sprite.visible = false;
			coinsCollected++;
		}
	}
}

bool GameApp::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;

	while (fixedElapsed >= FIXED_TIMESTEP)
	{
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	ProcessEvents();
	Collision();
	if (player.collidedBottom && keys[SDL_SCANCODE_SPACE])
		player.velocityY = 0.09f;
	Render(elapsed);
	return done;
}