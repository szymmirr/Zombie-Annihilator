#pragma once
#include "Effect.h"
#include "Model.h"
#include "Texture2D.h"
#include "Camera.hpp"
#include "RenderTarget2D.h"
#include "Quad.h"
#include "main.h"
#include "Zombie.h"
#include <time.h>
#include <conio.h>
using namespace std;

class Game {
	Quad* quad;
	Texture2D* tex;
	RenderTarget2D* rtTMO;
	RenderTarget2D* rt;

	Model* testModel;
	float posX;
	float posY;
	float posX2;
	float posY2;
	float posXbullet;
	float posYbullet;
	float posXzombie;
	float posYzombie;
	float rotate;
	float rotate2;
	float height;
	float speed;
	float prawo;
	float lewo;
	float gora;
	float dol;
	int shoot;
	int centerX = 1280 / 2;
	int centerY = 720 / 2;
	float bulletspeed;
	float angle;
	float delta;
	float rotatebullet;
	int respawn;
	float zombiespeed;
	float rotatezombie;
	float anglezombie;
	clock_t start, stop, start2;
	int zainit;
	Zombie z1;
	Zombie zombies[50];

	Model* sceneModelBox;
	float sceneWidth;
	float sceneHeight;
	int bullet;

	Model* myModel;
	Model* bulletModel;
	Model* zombieModel;
	Texture2D* myTex;
	Texture2D* grass;

	Effect* shaderColor;
	Effect* shaderTMO;
	Effect* shaderTextureDrawer;
	Camera camera;
	glm::vec3 LightPosition;
	float theta;
public:
	Game(void);
	~Game(void);
	void Update();
	void Init();
	void Redraw();
	void Input();
	void drawScene(void);
};
