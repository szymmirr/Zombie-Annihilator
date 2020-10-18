//#define GLM_FORCE_RADIANS

#include "Game.h"
#include "3rd\glm\gtc\matrix_transform.hpp"
#include "3rd\glm\glm.hpp"
#include "Content.h"
#include "Zombie.h"
#include "Keyboard.h"
#include "main.h"
#include <cmath>
#include <time.h>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
//#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

Game::Game(void)
{

}

Game::~Game(void)
{
}

// inicjalizacja parametrow renderingu
void Game::Init()
{
	// parametry polozenia i ruchu gracza
	sceneWidth = 16;
	sceneHeight = 14;
	rotate = -90;
	speed = 0.05f; // predkosc ruchu
	posX = 0.0f;
	posY = 0.0f;
	posX2 = 0;
	posY2 = 0;
	rotate2 = 0;
	height = 0;
	prawo = 0;
	lewo = 0;
	gora = 1;
	dol = 0;
	bullet = 0;
	shoot = 0;
	respawn = 0;
	bulletspeed = 0.26f;
	zombiespeed = 0.0015f;
	zainit = 0;
	start2 = clock();
	srand(time(NULL));
	z1.Init();
	
	for (int i = 0; i < 10; i++)
	{
		zombies[i].Init();
	}
	
	//PlaySound((LPCSTR) "C:\\kenny g.WAV", NULL, SND_FILENAME | SND_ASYNC);
	//PlaySound(TEXT("mywavsound.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//mciSendString("play wave1.wav", NULL, 0, NULL);
	
	//glMatrixMode(GL_PROJECTION); // switch to projection mode
	glLoadIdentity();
	glEnable(GL_FOG);
	float gl_fogcolor[] = { 0.875f, 0.957f, 1.0f, 1.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, gl_fogcolor);
	glFogf(GL_FOG_START, -90.0f);
	glFogf(GL_FOG_END, 18.0f);
	glFogf(GL_FOG_DENSITY, 999);

	theta = 1.0f;
	Effect::ShadersDirectory = "";
	glClearColor(0.6f, 0.5f, 1.0f, 1.0f);

	//stworzenie figury (quada) do wyœwietlania render targetów
	quad = new Quad();

	// wczytanie tekstury
	tex = Content::LoadTexture("../resources/test.png");

	// wczytanie modelu
	testModel = Content::LoadModel("../resources/ArmyPilot.obj");

	//ustawienie materia³u
	glm::mat4x4 mm;
	for (int i = 0; i < testModel->Elements(); i++)
	{
		//ustawienie koloru materia³u dla pobranego modelu
		testModel->GetMesh(i)->GetMaterial().Color = glm::vec3(1, 1, 0);
		//podmiana tekstury w modelu
		testModel->GetMesh(i)->GetMaterial().texture = tex;

		mm = testModel->GetMesh(i)->getLocalWorld();
	}

	// drugi model (sciany na scenie)
	sceneModelBox = Content::LoadModel("../resources/cube.ASE");
	for (int i = 0; i < sceneModelBox->Elements(); i++) {
		sceneModelBox->GetMesh(i)->GetMaterial().Color = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	// wczytanie tekstury
	myTex = Content::LoadTexture("../resources/gun.png");

	// wczytanie modelu
	myModel = Content::LoadModel("../resources/gun.3ds");
	bulletModel = Content::LoadModel("../resources/cube.ASE");
	zombieModel = Content::LoadModel("../resources/zombie2plus.3ds");
	grass = Content::LoadTexture("../resources/Grass.bmp");

	glm::mat4x4 mm2;
	for (int i = 0; i < myModel->Elements(); i++)
	{
		//ustawienie koloru materia³u dla pobranego modelu
		myModel->GetMesh(i)->GetMaterial().Color = glm::vec3(1, 1, 0);
		//podmiana tekstury w modelu
		myModel->GetMesh(i)->GetMaterial().texture = myTex;
		glBindTexture(GL_TEXTURE_2D, (GLuint) grass);

		mm2 = testModel->GetMesh(i)->getLocalWorld();
	}

	for (int i = 0; i < bulletModel->Elements(); i++)
	{
		//ustawienie koloru materia³u dla pobranego modelu
		bulletModel->GetMesh(i)->GetMaterial().Color = glm::vec3(1, 1, 0);
		//podmiana tekstury w modelu
		bulletModel->GetMesh(i)->GetMaterial().texture = myTex;
	}

	for (int i = 0; i < zombieModel->Elements(); i++)
	{
		//ustawienie koloru materia³u dla pobranego modelu
		zombieModel->GetMesh(i)->GetMaterial().Color = glm::vec3(1, 1, 0);
		//podmiana tekstury w modelu
		zombieModel->GetMesh(i)->GetMaterial().texture = myTex;
	}

	//////////////////////////////////////// RENDER TARGETS
	rtTMO = new RenderTarget2D(windowx, windowy, GL_RGBA8, GL_RGBA, GL_UNSIGNED_INT, GL_DEPTH24_STENCIL8);
	rt = new RenderTarget2D(windowx, windowy, GL_RGBA32F, GL_RGBA, GL_FLOAT, GL_DEPTH24_STENCIL8);

	/////////////////////////////////////// SHADERS
	// wczytanie i inicjalizacja shadera obliczajacego kolor powierzchni obiektu
	// wczytuje shadery (wspiera jedynie fragment (.ps) i vertex shadery (.vs), wystarczy podaæ nazwê pliku bez rozszerzenia)
	shaderColor = new Effect("shaders/color");
	// kompilacja shadera, alteratywnie mo¿na wykorzystaæ CreateShaderInfo, ktory wypisze w konsoli dane nt. shadera oraz ewnetualne b³êdy.
	shaderColor->CreateShader();

	// wczytanie i inicjalizacja shadera obliczajacego mapowanie tonow
	shaderTMO = new Effect("shaders/tmo");
	shaderTMO->CreateShader();

	// wczytanie i inicjalizacja shadera renderujacego obraz do framebuffera
	shaderTextureDrawer = new Effect("shaders/texture");
	shaderTextureDrawer->CreateShader();

	/////////////////////////////////////// KAMERA
	// ustawienie parametrów kamery (konstruktor przyjmuje rozdzielczoœæ viewportu),
	// domyœlny field of view to 45*, mo¿na zmieniæ metod¹ SetFOV(float)
	camera = Camera(windowx, windowy);
	camera.setPosition(glm::vec3(50, 50, 15));
	camera.setTarget(glm::vec3(50, 50, 50));
	//camera.setFOV( 60.0f );

	// polozenie zrodla swiatla
	LightPosition = glm::vec3(10, 10, 10);
}

// obsluga klawiatury, zmiana parametrow renderingu
void Game::Update()
{
	theta += 0.02f;
	if (Keyboard::isPressed('W'))
	{
		posY = posY + speed;
	}
	if (Keyboard::isPressed('S'))
	{
		posY = posY - speed;
	}
	if (Keyboard::isPressed('A'))
	{
		posX = posX - speed;
	}
	if (Keyboard::isPressed('D'))
	{
		posX = posX + speed;
	}
	if (Keyboard::isPressed('I')) {
		posY2 = posY2 + 0.07f;
	}
	if (Keyboard::isPressed('K')) {
		posY2 = posY2 - 0.07f;
	}
	if (Keyboard::isPressed('J')) {
		posX2 = posX2 - 0.07f;
	}
	if (Keyboard::isPressed('L')) {
		posX2 = posX2 + 0.07f;
	}
	if (Keyboard::isPressed('T')) {
		height += 0.1f;
	}
	if (Keyboard::isPressed('G')) {
		height -= 0.1f;
	}
	if (Keyboard::isPressed('N')) {
		bullet = 0;
		shoot = 0;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		if (shoot == 0)
		{
			bullet = 1;
			start = clock();
		}
	}

	if (Keyboard::isPressed(GLFW_KEY_UP)) {	
		posY = posY + speed;
	}
	if (Keyboard::isPressed(GLFW_KEY_DOWN)) {
		posY = posY - speed;
	}

	if (Keyboard::isPressed(GLFW_KEY_RIGHT)) {
		posX = posX + speed;
	}
	if (Keyboard::isPressed(GLFW_KEY_LEFT)) {
		posX = posX - speed;
	}

	// zmiana parametrow kamery w czasie ruchu
	camera.setPosition(glm::vec3(posX, posY, 10));
	float camTZ = -500;
	float camTY = posY - 3;
	float camTX = posX + 3;
	camera.setTarget(glm::vec3(camTX, camTY, camTZ));

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// display size
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	angle = atan2(centerY - ypos, centerX - xpos) * (180/3.14);
	rotate = angle + 180;
	anglezombie = atan2(posYzombie - posY, posXzombie - posX) * (180 / 3.14);

	if (Keyboard::isPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
	}
}

// rysowanie planszy gry skladajacej sie z obiektow sceneModelBox
void Game::drawScene(void) {
	int i = 0;
	shaderColor->GetParameter("matColor")->SetValue(sceneModelBox->GetMesh(i)->GetMaterial().Color);
	shaderColor->GetParameter("mode")->SetValue(-1.0f);

	for (int yy = 0; yy < this->sceneHeight; yy++)
		for (int xx = 0; xx < this->sceneWidth; xx++) {
			if (xx != 0 && xx != (this->sceneWidth - 1) && yy > 0 && yy < (this->sceneHeight - 1))
				continue;

			this->shaderColor->GetParameter("World")->SetValue(
				sceneModelBox->GetMesh(i)->getLocalWorld() *
				glm::translate(glm::mat4(1.0f), glm::vec3(-(float)this->sceneWidth / 2.0f, -(float)this->sceneHeight / 2.0f, 0.0f)) *
				glm::translate(glm::mat4(1.0f), glm::vec3((float)xx, 0.0f, 0.0f)) *
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (float)yy, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f))
			);

			// rysowanie modelu
			sceneModelBox->GetMesh(i)->Draw();
		}
}

// rysowanie sceny (glowna petla)
void Game::Redraw()
{
	//ustawienie textury do ktorej chcemy renderowaæ
	rt->SetRenderTarget();
	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ustawienie shadera e jako aktywnego
	shaderColor->Apply();
	//ustawienie macierzy widoku i projekcji
	shaderColor->GetParameter("View")->SetValue(camera.getView());
	shaderColor->GetParameter("Projection")->SetValue(camera.getProjection());
	//ustawienie pozycji œwiat³a i camery
	shaderColor->GetParameter("LightPosition")->SetValue(LightPosition);
	shaderColor->GetParameter("EyePosition")->SetValue(camera.getPosition());

	int i = 0;

	//ustawienie koloru materia³u
	shaderColor->GetParameter("matColor")->SetValue(testModel->GetMesh(i)->GetMaterial().Color);

	if (testModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*testModel->GetMesh(i)->GetMaterial().texture);
	}
	if (myModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*myModel->GetMesh(i)->GetMaterial().texture);
	}
	else
	{
		//w³¹czenie trybu bez tekstury
		shaderColor->GetParameter("mode")->SetValue(-1.0f);
	}

	if (bulletModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*bulletModel->GetMesh(i)->GetMaterial().texture);
	}

	if (zombieModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*zombieModel->GetMesh(i)->GetMaterial().texture);
	}

	// ustawienie transformacji obiektu
	// model mo¿e siê sk³adaæ z kilku czêœci (meshy) które znajduj¹ siê w przestrzeni lokalnej modelu,
	// musimy skorygowaæ ich pozycjê i przekszta³ciæ do przestrzeni naszego œwiata,
	// w tym celu wyci¹gamy macierz transformacji mesha testModel->GetMesh(i)->getLocalWorld(),
	// a nastêpnie przekszta³camy w podobny sposób jak w starszych wersjach OpenGL

	for (int i = 0; i < testModel->Elements(); i++) {

		shaderColor->GetParameter("World")->SetValue(
			testModel->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, 0)) *
			glm::rotate(glm::mat4(1.0f), -rotate+90, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.006, 0.006, 0.006))
		);
		// rysowanie modelu
		testModel->GetMesh(i)->Draw();
	}

	for (int i = 0; i < myModel->Elements(); i++) {

		this->shaderColor->GetParameter("World")->SetValue(
			myModel->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posX2, posY2, height)) *
			glm::rotate(glm::mat4(1.0f), -rotate2, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2))
		);

		// rysowanie modelu
		//myModel->GetMesh(i)->Draw();
	}

	for (int i = 0; i < bulletModel->Elements(); i++) {

		this->shaderColor->GetParameter("World")->SetValue(
			bulletModel->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posX2, posY2, height)) *
			glm::rotate(glm::mat4(1.0f), -rotate2, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.008, 0.008, 0.008))
		);
	}

	shaderColor->GetParameter("matColor")->SetValue(myModel->GetMesh(i)->GetMaterial().Color);

	if (myModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*myModel->GetMesh(i)->GetMaterial().texture);
	}
	else
	{
		//w³¹czenie trybu bez tekstury
		shaderColor->GetParameter("mode")->SetValue(-1.0f);
	}

	for (int i = 0; i < myModel->Elements(); i++) {

		shaderColor->GetParameter("World")->SetValue(

			myModel->GetMesh(i)->getLocalWorld() *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2))
		);
	}

	shaderColor->GetParameter("matColor")->SetValue(bulletModel->GetMesh(i)->GetMaterial().Color);

	if (bulletModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*bulletModel->GetMesh(i)->GetMaterial().texture);
	}
	else
	{
		//w³¹czenie trybu bez tekstury
		shaderColor->GetParameter("mode")->SetValue(-1.0f);
	}

	for (int i = 0; i < bulletModel->Elements(); i++) {

		if (bullet == 0 && shoot==0)
		{
			posXbullet = posX;
			posYbullet = posY;
			rotatebullet = rotate;
		}

		shaderColor->GetParameter("World")->SetValue(
			bulletModel->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posXbullet, posYbullet, height)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))
		);

		if (bullet == 1)
		{
			shoot = 1;
			posXbullet = posXbullet + bulletspeed * (float)cos(rotatebullet * 3.14 / 180.0f);
			posYbullet = posYbullet - bulletspeed * (float)sin(rotatebullet * 3.14 / 180.0f);
			// rysowanie modelu
			bulletModel->GetMesh(i)->Draw();
			if ((float(clock() - start) / CLOCKS_PER_SEC)>0.5)
			{
				bullet = 0;
				shoot = 0; 
			}
		}
	}

	shaderColor->GetParameter("matColor")->SetValue(zombieModel->GetMesh(i)->GetMaterial().Color);

	if (zombieModel->GetMesh(i)->GetMaterial().texture != NULL)
	{
		//jeœli mamy teksturê to j¹ bindujemy i ustawiamy tryb teskturowania w shaderze
		shaderColor->GetParameter("mode")->SetValue(1.0f);
		shaderColor->GetParameter("tex")->SetValue(*zombieModel->GetMesh(i)->GetMaterial().texture);
	}
	else
	{
		//w³¹czenie trybu bez tekstury
		shaderColor->GetParameter("mode")->SetValue(-1.0f);
	}

	for (int i = 0; i < 10; i++)
	{
		zombies[i].Update(zombieModel, posXbullet, posYbullet, posX, posY);
	}

	for (int i = 0; i < zombieModel->Elements(); i++)
	{
		if(respawn==0)
		{
			posXzombie = -3 + (rand() % static_cast<int>(3 - -3 + 1));
			posYzombie = -3 + (rand() % static_cast<int>(3 - -3 + 1));
		}

		posXzombie = posXzombie - zombiespeed * (float)cos(anglezombie * 3.14 / 180.0f);
		posYzombie = posYzombie - zombiespeed * (float)sin(anglezombie * 3.14 / 180.0f);

		shaderColor->GetParameter("World")->SetValue(
			zombieModel->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posXzombie, posYzombie, height)) *
			glm::rotate(glm::mat4(1.0f), anglezombie-90, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))
		);

		stop = clock();
		if ((start2)+1000<stop)
		{
			start2 = clock();
			respawn = 1;
		}
		if (respawn == 1)
		{
			if (posXbullet >= posXzombie - 0.5 && posXbullet <= posXzombie + 0.5)
			{
				if (posYbullet >= posYzombie - 0.5 && posYbullet <= posYzombie + 0.5)
				{
					respawn = 0;
				}
			}
		}
	}

	float size = 50.0f;
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glBegin(GL_QUADS);

		glTexCoord2f(size, 0.0f);
		glVertex3f(size, 0.0f, -size);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size, 0.0f, -size);

		glTexCoord2f(0.0f, size);
		glVertex3f(-size, 0.0f, size);

		glTexCoord2f(size, size);
		glVertex3f(size, 0.0f, size);

	glEnd();
	
	if (posX >= 6.7)
	{
		posX = 6.7;
	}
	if (posY >= 5.7)
	{
		posY = 5.7;
	}
	if (posX <= -7.7)
	{
		posX = -7.7;
	}
	if (posY <= -6.7)
	{
		posY = -6.7;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_QUADS);
#pragma region Podloga2
	{
		float m_amb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_dif[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-9.0f, 0.0f, -9.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-9.0f, 0.0f, 9.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(9.0f, 0.0f, 9.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(9.0f, 0.0f, -9.0f);
	}
#pragma endregion
	glEnd();

	drawScene();

	// mapowanie tonow za pomoca drugiego shadera, wynik renderowany jest do drugiego render targetu (rtTMO)
	rtTMO->SetRenderTarget();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderTMO->Apply();
	shaderTMO->GetParameter("tex")->SetValue(*rt);
	shaderTMO->GetParameter("gamma")->SetValue(1.6f);
	quad->Draw(0, 0, windowx, windowy, shaderTMO->GetParameter("World"));

	//rendering na ekran (do framebuffera)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowx, windowy);
	glClearColor(0.0, 0.0, 0.0, 0.0); //zmiana glClearColor by sprawdziæ czy faktycznie wyrenderowaliœmy coœ do tekstury
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderTextureDrawer->Apply();
	quad->Draw(0, 0, windowx, windowy, shaderTextureDrawer->GetParameter("World"));

}
