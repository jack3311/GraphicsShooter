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

	///
	/// initScene:
	/// Sets up a given scene
	/// _args - the scene's reset function arguments
	///
	template<typename T, typename ...Args>
	void initScene(Args ..._args);

	///
	/// activate:
	/// Activates a given scene
	/// _args - the scene's reset function arguments
	///
	template<typename T, typename ...Args>
	void activate(Args ..._args);

	///
	/// activateWithoutReset:
	/// Activates a given scene without resetting it
	///
	template<typename T>
	void activateWithoutReset();

	///
	/// getScene:
	/// returns a given scene instance
	///
	template<typename T>
	T & getScene();

	///
	/// render:
	/// Renders the currently activated scene
	///
	void render();

	///
	/// update:
	/// Updates the currently activated scene
	/// _dt - the delta time
	///
	void update(float _dt);
};

template<typename T, typename ...Args>
inline void SceneManager::initScene(Args ..._args)
{
	Logger::getLogger().log("Initilizing scene: ", typeid(T).name());

	T* scene = new T();
	//scene->reset(_args...);
	sceneMap[typeid(T)] = scene;
}

template<typename T, typename ...Args>
inline void SceneManager::activate(Args ..._args)
{
	Logger::getLogger().log("Activating scene: ", typeid(T).name());

	currentScene = sceneMap[typeid(T)];
	dynamic_cast<T*>(currentScene)->reset(_args...);
}

template<typename T>
inline void SceneManager::activateWithoutReset()
{
	Logger::getLogger().log("Activating scene: ", typeid(T).name());

	currentScene = sceneMap[typeid(T)];
}

template<typename T>
inline T & SceneManager::getScene()
{
	return *dynamic_cast<T*>(sceneMap[typeid(T)]);
}
