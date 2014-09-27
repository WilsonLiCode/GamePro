//Wilson Li SheetSprite Implementation
#include "SheetSprite.h"

SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(unsigned int textureID, float scale, float u, float v, float width, float height, float x, float y, float u1, float v1)
	: textureID(textureID), scale(scale), u(u), v(v), width(width), height(height), x(x), y(y), directionX(1.0f), u1(u1), v1(v1), curIndex(0), visible(true) {}

void SheetSprite::Draw()
{
	if (visible)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_TEXTURE_2D);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { -width*scale, height*scale, -width*scale, -height*scale, width*scale, -height*scale, width*scale, height*scale };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		if (curIndex <= 1)
		{
			GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
			glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		}
		else
		{
			GLfloat uvs[] = { u1, v1, u1, v1 + height, u1 + width, v1 + height, u1 + width, v1 };
			glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		}
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
}