//Wilson Li Bullet Implementation
#include "Bullet.h"

Bullet::Bullet()
	: maxTime(3.0f), angle(0.0f), size(0.01f), speed(-1.0f), visible(false) {}

Bullet::Bullet(float x, float y, float maxTime)
	: x(x), y(y), maxTime(maxTime), angle(0.0f), size(0.01f), speed(1.0f), visible(true), timeAlive(0.0f) {}

void Bullet::Update(float elapsed)
{
	if (timeAlive < maxTime)
	{
		y += cos(angle * PI / 180.0f) * elapsed * speed;
		timeAlive += elapsed;
	}
	else
		visible = false;
}

void Bullet::Draw()
{
	if (!visible)
		return;

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	GLfloat quad[] = { -size/2.0f, 3.0f*size, -size/2.0f, -3.0f*size, size/2.0f, -3.0f*size, size/2.0f, 3.0f*size };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}

void Bullet::Reset(float newX, float newY)
{
	x = newX;
	y = newY;
	timeAlive = 0.0f;
	visible = true;
}