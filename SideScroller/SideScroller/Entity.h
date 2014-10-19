//Wilson Li Entity Header
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SheetSprite.h"
#include "OtherFunctions.h"

#define FIXED_TIMESTEP 0.0166666f
#define TILE_SIZE 0.03f

class Entity
{
public:
	Entity();

	void Draw();
	bool collidesWith(Entity* otherEntity);
	void FixedUpdate();
	float getHeight();
	float getWidth();

	SheetSprite sprite;

	float x;
	float y;

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