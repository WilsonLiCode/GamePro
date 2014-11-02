//Wilson Li Entity Implementation
#include "Entity.h"

Entity::Entity()
	: x(0.0f), y(0.0f), rotation(0.0f), scaleX(1.0f), scaleY(1.0f), velocity(0.0f), acceleration(0.0f), friction(2.0f) {}

void Entity::Draw()
{
	if (sprite.visible)
	{
		BuildMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(matrix.ml);

		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLfloat quad[] = { -sprite.width, sprite.height, -sprite.width, -sprite.height, sprite.width, -sprite.height, sprite.width, sprite.height };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { sprite.u, sprite.v, sprite.u, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void Entity::BuildMatrix()
{
	Matrix translateMatrix;
	Matrix rotateMatrix;
	Matrix scaleMatrix;

	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(rotation);
	rotateMatrix.m[1][0] = -1.0f * sin(rotation);
	rotateMatrix.m[0][1] = sin(rotation);
	rotateMatrix.m[1][1] = cos(rotation);

	scaleMatrix.identity();
	scaleMatrix.m[0][0] = scaleX;
	scaleMatrix.m[1][1] = scaleY;

	matrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void Entity::FixedUpdate()
{
	if (sprite.visible)
	{
		velocity = lerp(velocity, 0.0f, FIXED_TIMESTEP * friction);
		velocity += acceleration * FIXED_TIMESTEP;
		rotationVelocity = lerp(rotationVelocity, 0.0f, FIXED_TIMESTEP * friction);
		rotationVelocity += rotationAcceleration * FIXED_TIMESTEP;

		rotation += rotationVelocity * FIXED_TIMESTEP;
		y += velocity * sin(rotation + (0.5f * PI)) * FIXED_TIMESTEP;
		x += velocity * cos(rotation + (0.5f * PI)) * FIXED_TIMESTEP;
	}
}

float Entity::getHeight()
{
	return sprite.height;
}

float Entity::getWidth()
{
	return sprite.width;
}
