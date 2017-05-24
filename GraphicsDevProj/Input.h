//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Input.h
//  Description  :    Manages user input
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2

class Input
{
public:
	static void init();

	static bool isKeyDown(const unsigned char _key);
	static bool isMouseDown(const unsigned int _button);

	static glm::vec2 getMousePosition();

private:
	Input() = delete;
	Input(const Input&) = delete;
	~Input() = delete;

	static bool mouseState[3];
	static bool keyState[255];

	static glm::vec2 mousePosition;

	static void keyboardFunc(unsigned char _key, int _x, int _y);
	static void keyboardFuncUp(unsigned char _key, int _x, int _y);
	
	static void mouseFunc(int _button, int _buttonState, int _x, int _y);
	static void mouseMotionFunc(int _x, int _y);
	static void mousePassiveFunc(int _x, int _y);
};

