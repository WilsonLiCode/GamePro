//Wilson Li SheetSprite Implementation
#include "SheetSprite.h"

SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(GLuint textureID, float scale, float u, float v, float width, float height)
	: textureID(textureID), scale(scale), u(u), v(v), width(width), height(height), visible(true), animated(false) {}

void SheetSprite::Draw()
{
	if (visible)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);

		GLfloat quad[] = { -width*scale, height*scale, -width*scale, -height*scale, width*scale, -height*scale, width*scale, height*scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
}

void SheetSprite::setAnimated(bool val, float fps, vector<float> framesU, vector<float> framesV)
{
	animated = val;
	framesPerSecond = fps;
	this->framesU = framesU;
	this->framesV = framesV;
	animationIndex = 0;
	animationElapsed = 0.0f;
}