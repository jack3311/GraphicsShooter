//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Input.cpp
//  Description  :    Manages user input
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Input.h"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Game.h"


bool Input::keyState[255];
bool Input::mouseState[3];
glm::vec2 Input::mousePosition;


void Input::init()
{
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardFuncUp);

	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(mousePassiveFunc);
	glutMotionFunc(mouseMotionFunc);
}

bool Input::isKeyDown(const unsigned char _key)
{
	assert(0 <= _key);
	assert(_key < 255);

	return keyState[_key];
}

bool Input::isMouseDown(const unsigned int _button)
{
	assert(0 <= _button);
	assert(_button < 3);

	return mouseState[_button];
}

glm::vec2 Input::getMousePosition() 
{
	return mousePosition;
}

void Input::keyboardFunc(unsigned char _key, int _x, int _y)
{
	if (_key < 0 || _key >= 255) return;

	keyState[_key] = true;
}

void Input::keyboardFuncUp(unsigned char _key, int _x, int _y)
{
	if (_key < 0 || _key >= 255) return;

	keyState[_key] = false;
}

void Input::mouseFunc(int _button, int _buttonState, int _x, int _y)
{
	//Works since _button variable also follows 0-1-2 system
	mouseState[_button] = _buttonState == GLUT_DOWN;

	mousePosition = glm::vec2(_x, Game::getGame()->getWindowHeight() - _y);
}

void Input::mouseMotionFunc(int _x, int _y)
{
	mousePosition = glm::vec2(_x, Game::getGame()->getWindowHeight() - _y);
}

void Input::mousePassiveFunc(int _x, int _y)
{
	mousePosition = glm::vec2(_x, Game::getGame()->getWindowHeight() - _y);
}
