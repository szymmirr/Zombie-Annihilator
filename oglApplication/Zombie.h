#pragma once
#include "Model.h"
#include "Texture2D.h"
#include "Effect.h"
#include <time.h>

class Zombie
{
public:
	Zombie(void);
	Model* zombieModel2;
	Texture2D* myTex2;
	Effect* shaderColor2;
	float posXzombie;
	float posYzombie;
	float zombiespeed;
	float anglezombie;
	clock_t start, stop, start2;
	int respawn;
	void Init();
	void Update(Model* zombieModel2, float posXbullet, float posYbullet, float posX, float posY);
	~Zombie(void);
};

