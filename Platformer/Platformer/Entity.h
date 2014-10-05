//Wilson Li Entity Header
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SheetSprite.h"
#include "OtherFunctions.h"

#define FIXED_TIMESTEP 0.0166666f

class Entity
{
public:
	Entity();

	void Draw(float elapsed);
	bool collidesWith(Entity* otherEntity);
	void FixedUpdate();

	SheetSprite sprite;

	float x;
	float y;

	float width;
	float height;

	float velocityX;
	float velocityY;

	float accelerationX;
	float accelerationY;

	float frictionX;
	float frictionY;

	bool isStatic;
	bool flip;

	bool enableCollisions;
	bool enableGravity;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};