//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneManager.h
//  Description  :    Manages the game scenes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <typeinfo>
#include <typeindex>

#include <unordered_map>

#include "Camera.h"
#include "SpriteRenderer.h"
#include "Logger.h"

class Scene;

class SceneManager
{
private:
	SceneManager();
	~SceneManager();


	static SceneManager * sceneManager; //Singleton class

	Scene * currentScene;

	std::unordered_map<std::type_index, Scene *> sceneMap;

public:
	static SceneManager & getSceneManager();
	static void deleteSceneManager();

	template<typename T, typename ...Args>
	void initScene(Args & ..._args);

	template<typename T, typename ...Args>
	void activate(Args & ..._args);

	template<typename T, typename ...Args>
	void activateWithoutReset(Args & ..._args);

	template<typename T>
	T & getScene();

	void render();
	void update(float _dt);
};

template<typename T, typename ...Args>
inline void SceneManager::initScene(Args & ..._args)
{
	Logger::getLogger().log("Initilizing scene: ", typeid(T).name());

	T* scene = new T();
	scene->reset(_args...);
	sceneMap[typeid(T)] = scene;
}

template<typename T, typename ...Args>
inline void SceneManager::activate(Args & ..._args)
{
	//currentScene = sceneMap[typeid(T)];
	dynamic_cast<T*>(currentScene)->reset(_args...);

	activateWithoutReset<T>(_args...);
}

template<typename T, typename ...Args>
inline void SceneManager::activateWithoutReset(Args & ..._args)
{
	Logger::getLogger().log("Activating scene: ", typeid(T).name());

	currentScene = sceneMap[typeid(T)];
}

template<typename T>
inline T & SceneManager::getScene()
{
	return *dynamic_cast<T*>(sceneMap[typeid(T)]);
}
