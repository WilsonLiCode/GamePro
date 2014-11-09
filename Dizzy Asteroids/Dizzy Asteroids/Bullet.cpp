//Wilson Li Bullet Implementation
#include "Bullet.h"

Bullet::Bullet()
	: maxTime(5.0f), angle(0.0f), size(0.1f), velocity(1.0f), visible(false) {}

Bullet::Bullet(float x, float y, float maxTime)
	: x(x), y(y), maxTime(maxTime), angle(0.0f), size(0.1f), velocity(1.0f), visible(true), timeAlive(0.0f) {}

void Bullet::Update()
{
	if (timeAlive < maxTime)
	{
		x += cos(angle + 0.5f * PI) * FIXED_TIMESTEP * velocity;
		y += sin(angle + 0.5f * PI) * FIXED_TIMESTEP * velocity;
		timeAlive += FIXED_TIMESTEP;
	}
	else
		visible = false;
}

void Bullet::BuildMatrix()
{
	Matrix translateMatrix;
	Matrix rotateMatrix;
	Matrix scaleMatrix;

	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(angle);
	rotateMatrix.m[1][0] = -1.0f * sin(angle);
	rotateMatrix.m[0][1] = sin(angle);
	rotateMatrix.m[1][1] = cos(angle);

	scaleMatrix.identity();
	scaleMatrix.m[0][0] = size;
	scaleMatrix.m[1][1] = size;

	matrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void Bullet::Draw()
{
	if (!visible)
		return;

	BuildMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);

	GLfloat point[] = { x, y };
	glVertexPointer(2, GL_FLOAT, 0, point);
	glEnableClientState(GL_VERTEX_ARRAY);

	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, 1);

	glPopMatrix();
}

void Bullet::Reset(float newX, float newY)
{
	x = newX;
	y = newY;
	timeAlive = 0.0f;
	visible = true;
}