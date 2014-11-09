//Wilson Li Particle Header
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Vector.h"

#define RANDOM_NUM ((float)rand()/(float)RAND_MAX)

using namespace std;

class Particle 
{
public:
	Particle();

	Vector position;
	Vector velocity;
	float lifetime;
};

class ParticleEmitter 
{
public:
	ParticleEmitter(unsigned int particleCount);
	ParticleEmitter();
	~ParticleEmitter();
	void Update(float elapsed);
	void Render();

	Vector position;
	Vector gravity;
	float maxLifetime;
	vector<Particle> particles;
};