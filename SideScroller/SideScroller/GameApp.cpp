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
	dead = false;

	fontText = loadTexture("pixel_font.png");
	spriteSheet = loadTexture("sprites.png");
	music = Mix_LoadMUS("music.ogg");
	Mix_PlayMusic(music, -1);
	jumpSound = Mix_LoadWAV("jump.wav");
	starSound = Mix_LoadWAV("star.wav");

	gravityX = 0.0f;
	gravityY = -1.0f;
	starsCollected = 0;

	BuildLevel();

	//Build player
	player.sprite = SheetSprite(spriteSheet, 159, SPRITE_COUNT_X, SPRITE_COUNT_Y);
	player.isStatic = false;
	player.enableGravity = true;
	entities.push_back(&player);
}

void GameApp::Init() 
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Side Scroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

void GameApp::BuildLevel()
{
	ifstream infile("tilemap.txt");
	string line;
	while (getline(infile, line)) 
	{
			if (line == "[header]") 
			{
				if (!readHeader(infile)) 
				{
					return;
				}
			}
			else if (line == "[layer]") 
			{
				readLayerData(infile);
			}
			else if (line == "[Object Layer 1]") 
			{
				readEntityData(infile);
			}
	}
}

bool GameApp::readHeader(ifstream &stream)
{
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) 
	{
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") 
			mapWidth = atoi(value.c_str());
		else if (key == "height")
			mapHeight = atoi(value.c_str());
	}
	
	if (mapWidth == -1 || mapHeight == -1)
		return false;
	else 
	{ 
		// allocate our map data
		levelData = new unsigned int*[mapHeight];
		for (int i = 0; i < mapHeight; ++i)
			levelData[i] = new unsigned int[mapWidth];
		return true;
	}
}
bool GameApp::readLayerData(ifstream &stream)
{
	string line;
	while (getline(stream, line)) 
	{
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") 
		{
			for (int y = 0; y < mapHeight; y++) 
			{
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) 
				{
					getline(lineStream, tile, ',');
					unsigned int val = (unsigned int)atoi(tile.c_str());
					if (val > 0)
						levelData[y][x] = val - 1;
					else
						levelData[y][x] = 0;
				}
			}
		}
	}
	return true;
}
bool GameApp::readEntityData(ifstream &stream)
{
	string line;
	string type;
	while (getline(stream, line)) 
	{
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") 
			type = value;
		else if (key == "location") 
		{
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void GameApp::placeEntity(string type, float placeX, float placeY)
{
	if (type == "player")
	{
		player.x = placeX;
		player.y = placeY;
	}
	else if(type == "star")
	{
		Entity* temp = new Entity();
		temp->sprite = SheetSprite(spriteSheet, 478, SPRITE_COUNT_X, SPRITE_COUNT_Y);
		temp->x = placeX;
		temp->y = placeY;
		temp->enableGravity = false;
		temp->enableCollisions = false;
		stars.push_back(temp);
		entities.push_back(temp);
	}
}

GameApp::~GameApp() 
{
	Mix_FreeChunk(jumpSound);
	Mix_FreeChunk(starSound);
	//Mix_FreeMusic(music);
	SDL_Quit();
}

void GameApp::Render() 
{
	glClearColor(0.878431f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (state == STATE_GAME)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spriteSheet);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float translateX = -player.x;
		float translateY = -player.y / 2;

		if (translateY < 0.0)
			translateY = 0.0;

		if (translateX > 0.0)
			translateX = 0.0;

		glTranslatef(translateX, translateY, 0.0f);

		vector<float> vertexData;
		vector<float> texCoordData;
		int numVertices = 0;

		for (int y = 0; y < LEVEL_HEIGHT; y++)
		{
			for (int x = 0; x < LEVEL_WIDTH; x++)
			{
				if (levelData[y][x] != 0)
				{
					float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
					float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
					float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
					float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
					vertexData.insert(vertexData.end(), {
						TILE_SIZE * x, -TILE_SIZE * y,
						TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
						(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
						(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
					});
					texCoordData.insert(texCoordData.end(), { u, v,
						u, v + (spriteHeight),
						u + spriteWidth, v + (spriteHeight),
						u + spriteWidth, v
					});
					numVertices += 4;
				}
			}
		}

		glPushMatrix();
		glTranslatef(TILE_SIZE * LEVEL_WIDTH / 2, -TILE_SIZE * LEVEL_HEIGHT / 2, 0.0f);

		glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

		glDrawArrays(GL_QUADS, 0, numVertices);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();

		for (int i = 0; i < entities.size(); i++)
			entities[i]->Draw();
	}
	else
	{
		glLoadIdentity();
		glTranslatef(-0.55f, 0.5f, 0.0f);
		DrawText(fontText, string("GAME OVER"), 0.15f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		if (dead)
		{
			glTranslatef(-0.2f, 0.0f, 0.0f);
			DrawText(fontText, string("You Died!"), 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (starsCollected < NUM_STARS)
		{
			glTranslatef(-0.8f, 0.0f, 0.0f);
			DrawText(fontText, string("You have failed to collect all the stars!"), 0.04f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			glTranslatef(-0.9f, 0.0f, 0.0f);
			DrawText(fontText, string("You have successfully collected all the stars!"), 0.04f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
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
		
		player.accelerationX = 0.0f;
		player.accelerationY = 0.0f;

		if (keys[SDL_SCANCODE_RIGHT])
		{
			player.flip = false;
			player.accelerationX = 1.5f;
		}
		else if (keys[SDL_SCANCODE_LEFT])
		{
			player.flip = true;
			player.accelerationX = -1.5f;
		}
	}
}

void GameApp::FixedUpdate()
{
	if (state != STATE_GAME)
		return;

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

		doLevelCollisionY(entities[i]);
		doLevelCollisionX(entities[i]);
	}
}

void GameApp::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) 
{
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}

bool GameApp::isSolid(unsigned int tile)
{
	//Mark the tiles that are solid
	switch (tile)
	{
	case 815:
	case 816:
	case 817:
	case 875:
	case 876:
	case 877:
	case 879:
	case 880:
	case 881:
	case 939:
	case 940:
	case 941:
	case 1058:
	case 1059:
	case 1060:
	case 1061:
	case 1121:
	case 1122:
	case 1123:
	case 1124:
	case 1125:
	case 1184:
	case 1185:
	case 1186:
	case 1187:
	case 1188:
	case 1189:
		return true;
		break;
	default:
		return false;
		break;
	}
}

float GameApp::checkPointForGridCollisionsY(float x, float y)
{
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 40)
		return 0.0f;

	if (isSolid(levelData[gridY][gridX]))
	{
		float yCoord = (gridY * TILE_SIZE) - (TILE_SIZE*0.0f);
		return -y - yCoord;
	}
	return 0.0;
}

float GameApp::checkPointForGridCollisionsX(float x, float y)
{
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 40)
		return 0.0f;

	if (isSolid(levelData[gridY][gridX]))
	{
		float xCoord = (gridX * TILE_SIZE) - (TILE_SIZE*0.0f);
		return x - xCoord;
	}
	return 0.0;
}

void GameApp::doLevelCollisionY(Entity* entity)
{
	//check bottom
	float adjust = checkPointForGridCollisionsY(entity->x, entity->y - entity->getHeight() * 0.5f);
	if (adjust != 0.0f)
	{
		entity->y += adjust;
		entity->velocityY = 0.0f;
		entity->collidedBottom = true;
	}

	//check top
	adjust = checkPointForGridCollisionsY(entity->x, entity->y + entity->getHeight() * 0.5f);
	if (adjust != 0.0f)
	{
		entity->y += adjust - TILE_SIZE;
		entity->velocityY = 0.0f;
		entity->collidedTop = true;
	}
}

void GameApp::doLevelCollisionX(Entity* entity)
{
	//check left
	float adjust = checkPointForGridCollisionsX(entity->x - entity->getWidth() * 0.5f, entity->y);
	if (adjust != 0.0f)
	{
		entity->x += adjust * TILE_SIZE;
		entity->velocityX = 0.0f;
		entity->collidedLeft = true;
	}

	//check right
	adjust = checkPointForGridCollisionsX(entity->x + entity->getWidth() * 0.5f, entity->y);
	if (adjust != 0.0f)
	{
		entity->x += (adjust - TILE_SIZE) * TILE_SIZE;
		entity->velocityX = 0.0f;
		entity->collidedRight = true;
	}
}


void GameApp::Collision()
{
	if (state != STATE_GAME)
		return;

	for (int i = 0; i < entities.size(); i++)
	{
		if (!entities[i]->isStatic && entities[i]->sprite.visible)
		{
			//y axis
			for (int j = 0; j < entities.size(); j++)
			{
				if (j != i && entities[j]->sprite.visible)
				{
					if (entities[i]->collidesWith(entities[j]))
					{
						float yPenetration = fabs(entities[i]->y - entities[j]->y) - entities[j]->getHeight() / 2.0f - entities[i]->getHeight() / 2.0f;
						if (entities[i]->y > entities[j]->y)
						{
							entities[i]->velocityY = 0.0f;
							entities[i]->collidedBottom = true;
							if (entities[i]->enableCollisions)
								entities[i]->y += yPenetration + 0.001f;
						}
						else
						{
							entities[i]->velocityY = 0.0f;
							entities[i]->collidedTop = true;
							if (entities[i]->enableCollisions)
								entities[i]->y -= yPenetration + 0.001f;
						}
					}
				}
			}
			
			//x axis
			for (int j = 0; j < entities.size(); j++)
			{
				if (j != i && entities[j]->sprite.visible)
				{
					if (entities[i]->collidesWith(entities[j]))
					{
						float xPenetration = fabs(entities[i]->x - entities[j]->x) - entities[j]->getWidth() / 2.0f - entities[i]->getWidth() / 2.0f;
						if (entities[i]->x > entities[j]->x)
						{
							entities[i]->velocityX = 0.0f;
							entities[i]->collidedLeft = true;
							if (entities[i]->enableCollisions)
								entities[i]->x += xPenetration + 0.001f;
						}
						else
						{
							entities[i]->velocityX = 0.0f;
							entities[i]->collidedRight = true;
							if (entities[i]->enableCollisions)
								entities[i]->x -= xPenetration + 0.001f;
						}
					}
				}
			}
		}
	}

	//Check
	for (int i = 0; i < stars.size(); i++)
	{
		if ((stars[i]->collidedBottom || stars[i]->collidedTop || stars[i]->collidedLeft || stars[i]->collidedRight) && stars[i]->sprite.visible)
		{
			stars[i]->sprite.visible = false;
			Mix_PlayChannel(-1, starSound, 0);
			starsCollected++;
		}
	}
	if (player.y < -1.3)
	{
		dead = true;
		state = STATE_GAMEOVER;
	}
	if (player.x > 6.03)
	{
		state = STATE_GAMEOVER;
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
		Collision();
	}
	timeLeftOver = fixedElapsed;

	ProcessEvents();
	if (player.collidedBottom && keys[SDL_SCANCODE_SPACE])
	{
		player.velocityY = 1.0f;
		Mix_PlayChannel(-1, jumpSound, 0);
	}
	Render();
	return done;
}