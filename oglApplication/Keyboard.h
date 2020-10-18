#pragma once

class Keyboard
{
	Keyboard(void);
public:
	static bool *keys;
	static void KeyboardInit();
	static void KeyboardKill();
	static bool isPressed(unsigned char key);
	static bool isPressed(int key);
	static void KeyPressFunc(unsigned char key, int x, int y);
	static void KeyUpFunc(unsigned char key, int x, int y);
	static void KeySpecPressFunc(int key, int x, int y);
	static void KeySpecUpFunc(int key, int x, int y);
	~Keyboard(void);
};

