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
	state = STATE_GAME;
	bulletIndex = 0;

	fontText = loadTexture("pixel_font.png");
	spriteText = loadTexture("sheet.png");

	//Build player
	player.sprite = SheetSprite(spriteText, 1.0f, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f);
	entities.push_back(&player);

	//Build asteroids
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		asteroids[i].sprite = SheetSprite(spriteText, 1.0f, 327.0f / 1024.0f, 452.0f / 1024.0f, 98.0f / 1024.0f, 96.0f / 1024.0f);
		asteroids[i].x = RANDOM_NUMBER;
		asteroids[i].y = RANDOM_NUMBER;
		float scale = (RANDOM_NUMBER + 2.0f);
		asteroids[i].scaleX = scale;
		asteroids[i].scaleY = scale;
		asteroids[i].acceleration = 1.0f;
		asteroids[i].rotation = RANDOM_NUMBER * 6.0f;
		entities.push_back(&asteroids[i]);
	}
}

void GameApp::Init() 
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
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

	if (state == STATE_GAME)
	{
		player.Draw();
		for (int i = 0; i < MAX_ASTEROIDS; i++)
			asteroids[i].Draw();
		for (int i = 0; i < MAX_BULLETS; i++)
			bullets[i].Draw();
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		Matrix tempMatrix;
		tempMatrix.identity();
		tempMatrix.m[3][0] = -0.6f;
		glMultMatrixf(tempMatrix.ml);
		DrawText(fontText, string("GAME OVER"), 0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glPopMatrix();
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
		else if (state == STATE_GAME && event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				if (bulletIndex == MAX_BULLETS)
					bulletIndex = 0;
				bullets[bulletIndex].angle = player.rotation;
				bullets[bulletIndex].Reset(player.x, player.y);
				bulletIndex++;
			}
		}

		if (state == STATE_GAME)
		{
			player.rotationAcceleration = 0.0f;
			player.acceleration = 0.0f;

			if (keys[SDL_SCANCODE_RIGHT])
			{
				player.rotationAcceleration = -5.0f;
			}
			else if (keys[SDL_SCANCODE_LEFT])
			{
				player.rotationAcceleration = 5.0f;
			}
			if (keys[SDL_SCANCODE_UP])
			{
				player.acceleration = 2.0f;
			}
		}
	}
}

void GameApp::FixedUpdate()
{
	if (state != STATE_GAME)
		return;

	for (int i = 0; i < MAX_BULLETS; i++)
		bullets[i].Update();

	//Update player
	player.FixedUpdate();
	player.collided = false;
	if (player.x < -1.33)
		player.x = 1.33f;
	else if (player.x > 1.33)
		player.x = -1.33f;
	if (player.y < -1)
		player.y = 1.0f;
	else if (player.y > 1)
		player.y = -1.0f;

	//Update asteroids
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		asteroids[i].FixedUpdate();
		asteroids[i].collided = false;
		if (asteroids[i].x < -1.33)
			asteroids[i].x = 1.33f;
		else if (asteroids[i].x > 1.33)
			asteroids[i].x = -1.33f;
		if (asteroids[i].y < -1)
			asteroids[i].y = 1.0f;
		else if (asteroids[i].y > 1)
			asteroids[i].y = -1.0f;
	}
}

void GameApp::Collision()
{
	if (state != STATE_GAME)
		return;

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->sprite.visible)
		{
			for (int j = 0; j < entities.size(); j++)
			{
				if (j != i && entities[j]->sprite.visible)
				{
					if (CheckCollision(entities[i], entities[j]))
					{
						Vector distance = Vector(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
						float length = distance.length();
						distance.normalize();
						entities[i]->collided = true;
						entities[j]->collided = true;

						entities[i]->x += distance.x * (0.001f / length);
						entities[i]->y += distance.y * (0.001f / length);
						entities[j]->x -= distance.x * (0.001f / length);
						entities[j]->y -= distance.y * (0.001f / length);
					}
				}
			}
		}
	}

	//Check bullet hits
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bullets[i].visible)
		{
			for (int j = 0; j < MAX_ASTEROIDS; j++)
			{
				if (asteroids[j].sprite.visible)
				{
					if (bullets[i].visible && CheckBullet(&bullets[i], &asteroids[j]))
					{
						bullets[i].visible = false;
						asteroids[j].sprite.visible = false;
					}
				}
			}
		}
	}

	//Check player hit
	//if (player.collided)
	//	state = STATE_GAMEOVER;

	//Check all asteroids destroyed
	bool won = true;
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (asteroids[i].sprite.visible)
			won = false;
	}
	if (won)
		state = STATE_GAMEOVER;
}

bool GameApp::CheckBullet(Bullet* bullet1, Entity* entity2)
{
	Matrix entityInverse = entity2->matrix.inverse();
	Vector bulletVector = Vector(bullet1->x, bullet1->y, 0.0f);

	bulletVector = entityInverse * bulletVector;
	if (bulletVector.x >= -entity2->getWidth() && bulletVector.x <= entity2->getWidth() && bulletVector.y >= -entity2->getHeight() && bulletVector.y <= entity2->getHeight())
		return true;
	else
		return false;
}

bool GameApp::CheckCollision(Entity* entity1, Entity* entity2)
{
	entity1->BuildMatrix();
	entity2->BuildMatrix();

	Matrix entity1inverse = entity1->matrix.inverse();
	Matrix entity2inverse = entity2->matrix.inverse();

	Vector ent1TL = Vector(-entity1->getWidth(), entity1->getHeight(), 0.0f);
	Vector ent1BL = Vector(-entity1->getWidth(), -entity1->getHeight(), 0.0f);
	Vector ent1TR = Vector(entity1->getWidth(), entity1->getHeight(), 0.0f);
	Vector ent1BR = Vector(entity1->getWidth(), -entity1->getHeight(), 0.0f);

	Vector ent2TL = Vector(-entity2->getWidth(), entity2->getHeight(), 0.0f);
	Vector ent2BL = Vector(-entity2->getWidth(), -entity2->getHeight(), 0.0f);
	Vector ent2TR = Vector(entity2->getWidth(), entity2->getHeight(), 0.0f);
	Vector ent2BR = Vector(entity2->getWidth(), -entity2->getHeight(), 0.0f);

	//transform entity2 to world space
	ent2TL = entity2->matrix * ent2TL;
	ent2BL = entity2->matrix * ent2BL;
	ent2TR = entity2->matrix * ent2TR;
	ent2BR = entity2->matrix * ent2BR;

	//transform entity2 to entity1's object space
	ent2TL = entity1inverse * ent2TL;
	ent2BL = entity1inverse * ent2BL;
	ent2TR = entity1inverse * ent2TR;
	ent2BR = entity1inverse * ent2BR;

	//check x axis
	float minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	float maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	if (minX > entity1->getWidth() || maxX < -entity1->getWidth())
		return false;

	//check y axis
	float minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	float maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	if (minY > entity1->getHeight() || maxY < -entity1->getHeight())
		return false;

	//reset entity2's vectors
	ent2TL = Vector(-entity2->getWidth(), entity2->getHeight(), 0.0f);
	ent2BL = Vector(-entity2->getWidth(), -entity2->getHeight(), 0.0f);
	ent2TR = Vector(entity2->getWidth(), entity2->getHeight(), 0.0f);
	ent2BR = Vector(entity2->getWidth(), -entity2->getHeight(), 0.0f);

	//transform entity1 to world space
	ent1TL = entity1->matrix * ent1TL;
	ent1BL = entity1->matrix * ent1BL;
	ent1TR = entity1->matrix * ent1TR;
	ent1BR = entity1->matrix * ent1BR;

	//transform entity1 to entity2's object space
	ent1TL = entity2inverse * ent1TL;
	ent1BL = entity2inverse * ent1BL;
	ent1TR = entity2inverse * ent1TR;
	ent1BR = entity2inverse * ent1BR;

	//check x axis
	minX = min(min(min(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	maxX = max(max(max(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	if (minX > entity2->getWidth() || maxX < -entity2->getWidth())
		return false;

	//check y axis
	minY = min(min(min(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	maxY = max(max(max(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	if (minY > entity2->getHeight() || maxY < -entity2->getHeight())
		return false;

	return true;
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
		Collision();
	}
	timeLeftOver = fixedElapsed;

	ProcessEvents();
	Render();
	return done;
}