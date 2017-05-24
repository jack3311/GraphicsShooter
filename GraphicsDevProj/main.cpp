//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    main.cpp
//  Description  :    Main file
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include <ctime>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Game.h"

#define START_WINDOW_WIDTH 800
#define START_WINDOW_HEIGHT 600
#define WINDOW_TITLE "Jack Wilson - Graphics Projects - 2017"

int numFrames = 0;

Game * g_game;

void render()
{
	g_game->render();
}

void update()
{
	g_game->update();
}

void reshape(int _width, int _height)
{
	glViewport(0, 0, _width, _height);
	g_game->setWindowDimensions(_width, _height);
}

int main(int argc, char ** argv)
{
	srand(static_cast<unsigned int>(time(0)));

	g_game = Game::createGame();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(START_WINDOW_WIDTH, START_WINDOW_HEIGHT);
	g_game->windowID = glutCreateWindow(WINDOW_TITLE);

	glewInit();

	g_game->init(START_WINDOW_WIDTH, START_WINDOW_HEIGHT);

	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutMainLoop();

	return 0;
}