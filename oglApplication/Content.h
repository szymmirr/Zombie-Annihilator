#pragma once
#include "Texture2D.h"
#include "Model.h"
#include <iostream> 
#include "3rd\GLFW\include\glfw3.h"
using namespace std;

class Content
{
public:
	Content(void);
	~Content(void);
	static Texture2D *LoadTexture(const char *filePath);
	static Model *LoadModel(const char *filePath);
};

