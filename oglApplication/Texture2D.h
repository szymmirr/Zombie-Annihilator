#pragma once
#include "3rd\GLFW\include\glfw3.h"

class Texture2D
{
	int width;
	int height;
	GLuint textureID;
public:
	Texture2D(void);
	Texture2D(int Width, int Height, GLuint TextureID);
	int GetWidth();
	int GetHeight();
	void Bind();
	~Texture2D(void);
};

