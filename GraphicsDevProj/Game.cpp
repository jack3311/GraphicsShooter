//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Game.cpp
//  Description  :    The main game class
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Game.h"

#include <iostream>
#include <sstream>

#include "SceneManager.h"
#include "Input.h"
#include "ShaderLoader.h"
#include "AssetManager.h"


Game * Game::game = nullptr;

Game::Game()
{
}

Game::~Game()
{
}

Game * Game::getGame()
{
	return game;
}

Game * Game::createGame()
{
	return game = new Game();
}

void Game::deleteGame()
{
	delete game;
}

Camera & Game::getCamera()
{
	return *camera;
}

SpriteRenderer & Game::getTexturedSpriteRenderer()
{
	return *texturedSpriteRenderer;
}

const GLuint & Game::getTexturedSpriteShader()
{
	return texturedSpriteShader;
}

void Game::fpsCounter()
{
	static int lastTime = 0;
	int timeForThisFrame = glutGet(GLUT_ELAPSED_TIME);
	float msThisFrame = static_cast<float>(timeForThisFrame) - lastTime;
	lastTime = timeForThisFrame;
	++numFrames;

	if (numFrames >= 1000)
	{
		numFrames = 0;
		std::cout << "Ms per frame: " << msThisFrame << std::endl;
		std::cout << "Equiv fps: " << (1000.f / msThisFrame) << std::endl;
	}
}

void Game::render()
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 1.0, 0.0, 1.0);

	SceneManager::getSceneManager().render();

	glutSwapBuffers();
}

void Game::update()
{
	finishedLoading = true;

	AssetManager::getAssetManager().update();

	fpsCounter();

	static float lastTimeSecs = 0;
	GLfloat currentTimeSecs = glutGet(GLUT_ELAPSED_TIME) / 1000.f;

	float dt = currentTimeSecs - lastTimeSecs;
	lastTimeSecs = currentTimeSecs;
	
	SceneManager::getSceneManager().update(dt);

	glutPostRedisplay();
}

void Game::init(int _windowWidth, int _windowHeight)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	//wglSwapIntervalEXT(0); //Disable vsync

	camera = new Camera(glm::vec2(0, 0));

	//LOAD SHADER
	ShaderLoader shaderLoader;
	texturedSpriteShader = shaderLoader.CreateProgram("Shaders/texturedSpriteShader.vert", "Shaders/texturedSpriteShader.frag");

	texturedSpriteRenderer = new SpriteRenderer(texturedSpriteShader);
	texturedSpriteRenderer->Initialise();

	AssetManager::getAssetManager().initSound();

	loadAssets();

	Input::init();
}

int Game::getWindowWidth() const
{
	return windowWidth;
}

int Game::getWindowHeight() const
{
	return windowHeight;
}

void Game::setWindowDimensions(int _width, int _height)
{
	windowWidth = _width;
	windowHeight = _height;

	camera->updateCamera();
}

bool Game::hasFinishedLoading() const
{
	return finishedLoading;
}

void Game::loadAssets()
{
	AssetManager::getAssetManager().loadTexture("star", "Assets/Textures/Star.png", true);
	
	AssetManager::getAssetManager().loadTexture("laser-1", "Assets/Textures/laser/1.png", true);
	AssetManager::getAssetManager().loadTexture("laser-2", "Assets/Textures/laser/2.png", true);

	AssetManager::getAssetManager().loadTexture("bullet1", "Assets/Textures/bullet1.png", true);
	AssetManager::getAssetManager().loadTexture("bullet2", "Assets/Textures/bullet2.png", true);

	AssetManager::getAssetManager().loadTexture("ship1", "Assets/Textures/playerShip2_blue.png", true);
	AssetManager::getAssetManager().loadTexture("enemy1", "Assets/Textures/enemyRed2.png", true);
	AssetManager::getAssetManager().loadTexture("boss", "Assets/Textures/ufoYellow.png", true);

	AssetManager::getAssetManager().loadTexture("black", "Assets/Textures/black.jpg");

	AssetManager::getAssetManager().loadTexture("planet1", "Assets/Textures/planets/1.png", true);
	AssetManager::getAssetManager().loadTexture("planet2", "Assets/Textures/planets/2.png", true);
	AssetManager::getAssetManager().loadTexture("planet3", "Assets/Textures/planets/3.png", true);

	AssetManager::getAssetManager().loadCubeMap("skybox1", std::vector<std::string>{
			"Assets/Textures/envmap_miramar/miramar_rt.png",
			"Assets/Textures/envmap_miramar/miramar_lf.png",
			"Assets/Textures/envmap_miramar/miramar_up.png",
			"Assets/Textures/envmap_miramar/miramar_dn.png",
			"Assets/Textures/envmap_miramar/miramar_bk.png",
			"Assets/Textures/envmap_miramar/miramar_ft.png",
		}
	);
/*
	AssetManager::getAssetManager().loadSound("explode1", "Assets/Sounds/explode1.wav");
	AssetManager::getAssetManager().loadSound("explode2", "Assets/Sounds/explode2.wav");
	AssetManager::getAssetManager().loadSound("laser", "Assets/Sounds/laser.wav");
	AssetManager::getAssetManager().loadSound("fail", "Assets/Sounds/fail.wav");
	AssetManager::getAssetManager().loadSound("success", "Assets/Sounds/success.wav");
	AssetManager::getAssetManager().loadSound("star", "Assets/Sounds/star.wav");*/
}
