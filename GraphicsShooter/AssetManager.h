//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    AssetManager.h
//  Description  :    Manages all program assets
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\FMOD\fmod.hpp"

#include <unordered_map>
#include <string>

#define MAX_AUDIO_CHANNELS 50

class AssetManager
{
public:
	static AssetManager & getAssetManager();
	static void deleteAssetManager();

	GLuint loadTextureSimple(const std::string & _path, const bool _alphaEnabled = false);

	GLuint loadTexture(const std::string & _name, const std::string & _path, const bool _alphaEnabled = false);
	GLuint getTexture(const std::string & _name);

	GLuint loadCubeMap(const std::string & _name, const std::vector<std::string> & _paths, const bool _alphaEnabled = false);
	GLuint getCubeMap(const std::string & _name);

	FMOD::Sound * loadSound(const std::string & _name, const std::string & _path, const FMOD_MODE & _mode = FMOD_INIT_NORMAL);
	FMOD::Sound * getSound(const std::string & _name);

	FMOD::Channel * playSound(FMOD::Sound * _sound);

	bool initSound();

	void update();

private:
	std::unordered_map<std::string, GLuint> textureMap;
	std::unordered_map<std::string, GLuint> cubeMapMap;
	std::unordered_map<std::string, FMOD::Sound *> soundMap;

	FMOD::System * audioManager;

	static AssetManager * assetManager; //Singleton class

	AssetManager();
	~AssetManager();
	AssetManager(const AssetManager&) = delete;
};

