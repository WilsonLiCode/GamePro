//Wilson Li Entity Implementation
#include "Entity.h"

Entity::Entity()
	: x(0.0f), y(0.0f), velocityX(0.0f), velocityY(0.0f), flip(false), isStatic(false), frictionX(2.0f), frictionY(2.0f), enableGravity(true), enableCollisions(true) {}

void Entity::Draw(float elapsed)
{
	if (sprite.visible && sprite.animated && !flip)
	{
		sprite.animationElapsed += elapsed;
		if (sprite.animationElapsed > 1.0 / sprite.framesPerSecond)
		{
			sprite.animationIndex++;
			sprite.animationElapsed = 0.0f;
			if (sprite.animationIndex > sprite.framesU.size() - 1)
			{
				sprite.animationIndex = 0;
			}
		}

		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { -sprite.width*sprite.scale, sprite.height*sprite.scale, -sprite.width*sprite.scale, -sprite.height*sprite.scale, sprite.width*sprite.scale, -sprite.height*sprite.scale, sprite.width*sprite.scale, sprite.height*sprite.scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { sprite.framesU[sprite.animationIndex], sprite.framesV[sprite.animationIndex], sprite.framesU[sprite.animationIndex], sprite.framesV[sprite.animationIndex] + sprite.height, sprite.framesU[sprite.animationIndex] + sprite.width, sprite.framesV[sprite.animationIndex] + sprite.height, sprite.framesU[sprite.animationIndex] + sprite.width, sprite.framesV[sprite.animationIndex] };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
	else if (sprite.visible && sprite.animated && flip)
	{
		sprite.animationElapsed += elapsed;
		if (sprite.animationElapsed > 1.0 / sprite.framesPerSecond)
		{
			sprite.animationIndex++;
			sprite.animationElapsed = 0.0f;
			if (sprite.animationIndex > sprite.framesU.size() - 1)
			{
				sprite.animationIndex = 0;
			}
		}

		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { sprite.width*sprite.scale, sprite.height*sprite.scale, sprite.width*sprite.scale, -sprite.height*sprite.scale, -sprite.width*sprite.scale, -sprite.height*sprite.scale, -sprite.width*sprite.scale, sprite.height*sprite.scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { sprite.framesU[sprite.animationIndex], sprite.framesV[sprite.animationIndex], sprite.framesU[sprite.animationIndex], sprite.framesV[sprite.animationIndex] + sprite.height, sprite.framesU[sprite.animationIndex] + sprite.width, sprite.framesV[sprite.animationIndex] + sprite.height, sprite.framesU[sprite.animationIndex] + sprite.width, sprite.framesV[sprite.animationIndex] };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
	else if (sprite.visible && !sprite.animated && !flip)
	{
		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { -sprite.width*sprite.scale, sprite.height*sprite.scale, -sprite.width*sprite.scale, -sprite.height*sprite.scale, sprite.width*sprite.scale, -sprite.height*sprite.scale, sprite.width*sprite.scale, sprite.height*sprite.scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { sprite.u, sprite.v, sprite.u, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
	else if (sprite.visible && !sprite.animated && flip)
	{
		glBindTexture(GL_TEXTURE_2D, sprite.textureID);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { sprite.width*sprite.scale, sprite.height*sprite.scale, sprite.width*sprite.scale, -sprite.height*sprite.scale, -sprite.width*sprite.scale, -sprite.height*sprite.scale, -sprite.width*sprite.scale, sprite.height*sprite.scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { sprite.u, sprite.v, sprite.u, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v + sprite.height, sprite.u + sprite.width, sprite.v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
}

bool Entity::collidesWith(Entity* otherEntity)
{
	float thisTop = this->y;
	float thisBottom = this->y;
	float thisLeft = this->x;
	float thisRight = this->x;
	float otherTop = otherEntity->y + otherEntity->sprite.height / 1.1f;
	float otherBottom = otherEntity->y - otherEntity->sprite.height / 1.1f;
	float otherLeft = otherEntity->x - otherEntity->sprite.width / 1.1f;
	float otherRight = otherEntity->x + otherEntity->sprite.width / 1.1f;
	
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
	}
}
