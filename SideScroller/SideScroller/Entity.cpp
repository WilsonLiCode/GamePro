//Wilson Li Entity Implementation
#include "Entity.h"

Entity::Entity()
	: x(0.0f), y(0.0f), velocityX(0.0f), velocityY(0.0f), accelerationX(0.0f), accelerationY(0.0f), flip(false), isStatic(false), frictionX(2.0f), frictionY(2.0f), enableGravity(true), enableCollisions(true) {}

void Entity::Draw()
{
	float u = (float)(((int)sprite.index) % sprite.spriteCountX) / (float)sprite.spriteCountX;
	float v = (float)(((int)sprite.index) / sprite.spriteCountX) / (float)sprite.spriteCountY;
	float spriteWidth = 1.0f / (float)sprite.spriteCountX;
	float spriteHeight = 1.0f / (float)sprite.spriteCountY;

	if (sprite.visible && !flip)
	{
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { -TILE_SIZE, TILE_SIZE, -TILE_SIZE, -TILE_SIZE, TILE_SIZE, -TILE_SIZE, TILE_SIZE, TILE_SIZE };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else if (sprite.visible && flip)
	{
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { TILE_SIZE, TILE_SIZE, TILE_SIZE, -TILE_SIZE, -TILE_SIZE, -TILE_SIZE, -TILE_SIZE, TILE_SIZE };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

bool Entity::collidesWith(Entity* otherEntity)
{
	float spriteWidth = 1.0f / (float)sprite.spriteCountX;
	float spriteHeight = 1.0f / (float)sprite.spriteCountY;

	float thisTop = this->y + spriteHeight / 2.0f;
	float thisBottom = this->y - spriteHeight / 2.0f;
	float thisLeft = this->x - spriteWidth / 2.0f;
	float thisRight = this->x + spriteWidth / 2.0f;
	float otherTop = otherEntity->y + otherEntity->getHeight() / 2.0f;
	float otherBottom = otherEntity->y - otherEntity->getHeight() / 2.0f;
	float otherLeft = otherEntity->x - otherEntity->getWidth() / 2.0f;
	float otherRight = otherEntity->x + otherEntity->getWidth() / 2.0f;
	
	if (thisBottom > otherTop || thisTop < otherBottom || thisLeft > otherRight || thisRight < otherLeft)
		return false;
	else
		return true;
}


void Entity::FixedUpdate()
{
	if (!isStatic && sprite.visible)
	{
		velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * frictionX);
		velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * frictionY);
		velocityX += accelerationX * FIXED_TIMESTEP;
		velocityY += accelerationY * FIXED_TIMESTEP;

		y += velocityY * FIXED_TIMESTEP;
		x += velocityX * FIXED_TIMESTEP;
	}
}

float Entity::getHeight()
{
	return 1.0f / (float)sprite.spriteCountY;
}

float Entity::getWidth()
{
	return 1.0f / (float)sprite.spriteCountX;
}
