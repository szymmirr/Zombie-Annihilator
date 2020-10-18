// oglApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "3rd\glew\include\GL\glew.h"
#include "3rd\glm\glm.hpp"
#include "3rd\GLFW\include\glfw3.h"
#include "Game.h"
#include "Content.h"
#include "Effect.h"
#include "Keyboard.h"
#include "main.h"


int _tmain(int argc, _TCHAR* argv[]) {
	
	kills = 0;
	// inicjlizacja GLFW
	if (!glfwInit()) {
	    return -1;
	}
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 2);

	window = glfwCreateWindow( windowx, windowy, "Zombie Annihilator", NULL, NULL);
	if ( !window ) {
	    glfwTerminate();
	    return -1;
	}	
	glfwMakeContextCurrent(window);

	GLenum err = glGetError(); 
	if( err ) {
		fprintf( stderr, "ERROR: %s\n", glewGetErrorString( err ) );
		exit(EXIT_FAILURE);
	}

	// inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult;
	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf( stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	// glowna petla gry
	Game game;
	game.Init();
	Keyboard::KeyboardInit();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
	    // rendering
		game.Update();
		game.Redraw();

	    glfwSwapBuffers(window);
	
	    glfwPollEvents();
	}
	
	glfwTerminate();
	cout << "Koniec gry!" << endl << endl << "Zabiles " << kills-10 << " zombie" << endl;
	cin.get();
	cin.get();

	return 0;
}

