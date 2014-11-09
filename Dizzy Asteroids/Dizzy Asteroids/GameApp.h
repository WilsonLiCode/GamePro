//Wilson Li GameApp Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <algorithm>
#include <string>
#include "OtherFunctions.h"
#include "Entity.h"
#include "SheetSprite.h"
#include "Bullet.h"
#include "Particle.h"

#define MAX_TIMESTEPS 6
#define MAX_ASTEROIDS 6
#define MAX_BULLETS 20
#define RANDOM_NUMBER ((float)rand()/((float)RAND_MAX/2.0f) - 1.0f)

using namespace std;

class GameApp 
{
public:
	GameApp();
	~GameApp();
	void Init();
	bool UpdateAndRender();
	void Render();
	void ProcessEvents();
	void Update(float elapsed);
	void FixedUpdate();
	void Collision();
	bool CheckCollision(Entity* entity1, Entity* entity2);
	bool CheckBullet(Bullet* bullet1, Entity* entity2);

private:
	const Uint8 *keys;
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	float animationTime;
	float screenShakeValue;
	float screenShakeSpeed;
	float screenShakeIntensity;
	ParticleEmitter gameParticles;

	int state;
	bool dead;

	GLuint fontText;
	GLuint spriteText;

	Entity player;
	Entity asteroids[MAX_ASTEROIDS];
	vector<Entity*> entities;
	Bullet bullets[MAX_BULLETS];
	int bulletIndex;
};