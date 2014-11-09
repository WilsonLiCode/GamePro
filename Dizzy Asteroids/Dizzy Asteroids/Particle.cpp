//Wilson Li Particle Implementation
#include "Particle.h"

Particle::Particle() {}

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::~ParticleEmitter() {}

ParticleEmitter::ParticleEmitter(unsigned int particleCount)
{
	particles.resize(particleCount);
	maxLifetime = 2.0f;
	for (int i = 0; i<particles.size(); i++)
		particles[i].lifetime = RANDOM_NUM * maxLifetime;

	gravity = Vector(0.0f, -5.0f, 0.0f);
}

void ParticleEmitter::Update(float elapsed)
{
	for (int i = 0; i<particles.size(); i++)
	{
		particles[i].velocity.x += gravity.x * elapsed;
		particles[i].velocity.y += gravity.y * elapsed;
		particles[i].position.x += particles[i].velocity.x * elapsed;
		particles[i].position.y += particles[i].velocity.y * elapsed;
		particles[i].lifetime += elapsed;

		if (particles[i].lifetime > maxLifetime)
		{
			particles[i].lifetime = 0.0f;

			//linear velocity
			particles[i].velocity = Vector();

			particles[i].position = position;
		}
	}
}

void ParticleEmitter::Render()
{
	vector<float> particleVertices;
	for (int i = 0; i < particles.size(); i++) 
	{
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}
	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glPointSize(3.0);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
}