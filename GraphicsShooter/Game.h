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

#include <string>

class SpriteRenderer;
class Camera;
class GameWorld;

class Game
{
private:
	static Game * game;

	Game();
	Game(const Game &) = delete;
	~Game();

	Camera * camera;

	int numFrames = 0;

	int windowWidth, windowHeight;

	void fpsCounter();
	void loadAssets();

	bool finishedLoading = false;

	GameWorld * gameWorld = nullptr;

public:
	int windowID;

	static Game * getGame();
	static void deleteGame();
	static Game * createGame();

	Camera & getCamera();

	/// 
	/// render:
	/// Renders the current scene
	///
	void render();

	/// 
	/// update:
	/// Updates the current scene
	///
	void update();

	/// 
	/// init:
	/// Initialises the game
	/// _windowWidth - the width of the window
	/// _windowHeight - the height of the window
	///
	void init(int _windowWidth, int _windowHeight);

	int getWindowWidth() const;
	int getWindowHeight() const;

	/// 
	/// setWindowDimensions:
	/// Updates the dimensions of the game window
	/// _width - the width of the window
	/// _height - the height of the window
	///
	void setWindowDimensions(int _width, int _height);

	/// 
	/// hasFinishedLoading:
	/// returns whether all scenes have been loaded
	///
	bool hasFinishedLoading() const;


	/// 
	/// createGameWorld:
	/// Creates a game world
	///
	void createGameWorld(bool _isServer, bool _isMultiplayer, std::string _username, std::string _serverName = "");


	/// 
	/// deleteGameWorld:
	/// Deletes the game world
	///
	void deleteGameWorld();

	/// 
	/// getGameWorld:
	/// returns the game world
	///
	GameWorld & getGameWorld() const;

	/// 
	/// hasGameWorld:
	/// returns whether a game world exists
	///
	bool hasGameWorld() const;
};

