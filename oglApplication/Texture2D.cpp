
#include "Texture2D.h"


Texture2D::Texture2D(void)
{
}

Texture2D::Texture2D(int Width, int Height, GLuint TextureID)
{
	width = Width;
	height = Height;
	textureID = TextureID;
}


Texture2D::~Texture2D(void)
{
}

int Texture2D::GetWidth()
{
	return height;
}

int Texture2D::GetHeight()
{
	return width;
}

void Texture2D::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}
