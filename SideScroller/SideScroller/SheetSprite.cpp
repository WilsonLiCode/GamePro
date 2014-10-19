//Wilson Li SheetSprite Implementation
#include "SheetSprite.h"

SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(GLuint textureID, int index, int spriteCountX, int spriteCountY)
	: textureID(textureID), index(index), spriteCountX(spriteCountX), spriteCountY(spriteCountY), visible(true), animated(false) {}

void SheetSprite::Draw()
{
	if (visible)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);

		GLfloat quad[] = { -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		float spriteWidth = 1.0f / (float)spriteCountX;
		float spriteHeight = 1.0f / (float)spriteCountY;
		GLfloat quadUVs[] = { u, v,
			u, v + spriteHeight,
			u + spriteWidth, v + spriteHeight,
			u + spriteWidth, v};
		
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
}

void SheetSprite::setAnimated(bool val, float fps, vector<float> frames)
{
	animated = val;
	framesPerSecond = fps;
	this->frames = frames;
	animationIndex = 0;
	animationElapsed = 0.0f;
}