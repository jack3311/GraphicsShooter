//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneManager.cpp
//  Description  :    Manages the game scenes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "SceneManager.h"

#include "Scene.h"
#include "SceneMenu.h"
#include "ScenePlay.h"
#include "SceneGameOver.h"
#include "SceneMP.h"
#include "SceneHelp.h"

SceneManager * SceneManager::sceneManager = nullptr;

SceneManager::SceneManager()
{
	initScene<SceneMenu>();
	initScene<ScenePlay>();
	initScene<SceneGameOver>();
	initScene<SceneMP>();
	initScene<SceneHelp>();

	activate<SceneMenu>();
}


SceneManager::~SceneManager()
{
	for (std::pair<std::type_index, Scene *> s : sceneMap)
	{
		delete s.second;
	}

	sceneMap.clear();
}

SceneManager & SceneManager::getSceneManager()
{
	if (!SceneManager::sceneManager)
	{
		sceneManager = new SceneManager();
	}

	return *sceneManager;
}

void SceneManager::deleteSceneManager()
{
	delete sceneManager;
}

void SceneManager::render()
{
	currentScene->render();
}

void SceneManager::update(float _dt)
{
	currentScene->update(_dt);
}
