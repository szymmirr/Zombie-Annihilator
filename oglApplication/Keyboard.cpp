#include "stdafx.h"
#include "Keyboard.h"
#include "3rd\GLFW\include\glfw3.h"

Keyboard::Keyboard(void)
{
}

void Keyboard::KeyboardInit(void)
{
	if(keys == NULL)
	Keyboard::keys = new bool[256+246];
	for(int i = 0; i < 256+246; i++)
	{
		keys[i] = false;
	}
}
void Keyboard::KeyPressFunc(unsigned char key, int x, int y)
{
	keys[key] = true;
}
void Keyboard::KeyUpFunc(unsigned char key, int x, int y)
{
	keys[key] = false;
}


void Keyboard::KeySpecPressFunc(int key, int x, int y)
{
	keys[key+255] = true;
}
void Keyboard::KeySpecUpFunc(int key, int x, int y)
{
	keys[key+255] = false;
}


bool Keyboard::isPressed(unsigned char key)
{
	return glfwGetKey(glfwGetCurrentContext(),key) == GLFW_PRESS;
	return keys[key];
}
bool Keyboard::isPressed(int key)
{
	return glfwGetKey(glfwGetCurrentContext(),key) == GLFW_PRESS;
	return keys[255+key];
}

Keyboard::~Keyboard(void)
{

}
void Keyboard::KeyboardKill(void)
{
	delete keys;
}
bool *Keyboard::keys = new bool[256+246];
