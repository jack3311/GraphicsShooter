//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Game.h
//  Description  :    The main game class
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glew\wglew.h"

class SpriteRenderer;
class Camera;

class Game
{
private:
	static Game * game;

	Game();
	Game(const Game &) = delete;
	~Game();

	SpriteRenderer * texturedSpriteRenderer;
	Camera * camera;

	GLuint texturedSpriteShader;

	int numFrames = 0;

	int windowWidth, windowHeight;

	void fpsCounter();
	void loadAssets();

	bool finishedLoading = false;

public:
	int windowID;

	static Game * getGame();
	static void deleteGame();
	static Game * createGame();

	Camera & getCamera();
	SpriteRenderer & getTexturedSpriteRenderer();

	const GLuint & getTexturedSpriteShader();

	void render();
	void update();
	void init(int _windowWidth, int _windowHeight);

	int getWindowWidth() const;
	int getWindowHeight() const;

	void setWindowDimensions(int _width, int _height);

	bool hasFinishedLoading() const;
};

