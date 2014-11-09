//Wilson Li OtherFunctions Implementation
#include "OtherFunctions.h"

GLuint loadTexture(const char *image_path, GLenum format)
{
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawText(int textureID, string text, float size, float spacing, float r, float g, float b, float a)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	float textureSize = 1.0f / 16.0f;

	for (int i = 0; i < text.size(); i++)
	{
		float textureX = (float)((int)text[i] % 16) / 16.0f;
		float textureY = (float)((int)text[i] / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		uvData.insert(uvData.end(), { textureX, textureY, textureX, textureY + textureSize, textureX + textureSize, textureY + textureSize, textureX + textureSize, textureY });
	}
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);
}

void DrawSprite(GLint texture, float x, float y, float scale, bool additive)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix translateMatrix;
	Matrix scaleMatrix;
	Matrix matrix;

	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	scaleMatrix.identity();
	scaleMatrix.m[0][0] = scale;
	scaleMatrix.m[1][1] = scale;

	matrix = scaleMatrix * translateMatrix;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);

	GLfloat quad[] = { 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (additive)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

float lerp(float v0, float v1, float t) 
{
	return (1.0f - t)*v0 + t*v1;
}

//Tweening Effects Functions
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) 
{
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin)
		retVal = dstMin;
	if (retVal > dstMax)
		retVal = dstMax;
	return retVal;
}

float easeIn(float from, float to, float time) 
{
	float tVal = time*time*time*time*time;
	return (1.0f - tVal)*from + tVal*to;
}

float easeOut(float from, float to, float time) 
{
	float oneMinusT = 1.0f - time;
	float tVal = 1.0f - (oneMinusT * oneMinusT * oneMinusT * oneMinusT * oneMinusT);
	return (1.0f - tVal)*from + tVal*to;
}

float easeInOut(float from, float to, float time) 
{
	float tVal;
	if (time > 0.5) 
	{
		float oneMinusT = 1.0f - ((0.5f - time)*-2.0f);
		tVal = 1.0f - ((oneMinusT * oneMinusT * oneMinusT * oneMinusT * oneMinusT) * 0.5f);
	}
	else 
	{
		time *= 2.0f;
		tVal = (time*time*time*time*time) / 2.0f;
	}
	return (1.0f - tVal)*from + tVal*to;
}

float easeOutElastic(float from, float to, float time) 
{
	float p = 0.3f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s)*(2 * PI) / p));
}