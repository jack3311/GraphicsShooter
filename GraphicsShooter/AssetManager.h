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

	/// 
	/// loadTextureSimple:
	/// Directly loads a texture
	/// _path - the path to the texture file
	/// _alphaEnabled - whether the texture contains an alpha channel
	/// returns the address of the texture
	///
	GLuint loadTextureSimple(const std::string & _path, const bool _alphaEnabled = false);

	/// 
	/// loadTexture:
	/// Loads a texture with an associated name
	/// _name - the name for the texture
	/// _path - the path to the texture file
	/// _alphaEnabled - whether the texture contains an alpha channel
	/// returns the address of the texture
	///
	GLuint loadTexture(const std::string & _name, const std::string & _path, const bool _alphaEnabled = false);

	/// 
	/// getTexture:
	/// Gets a texture associated with a given name
	/// _name - the name for the texture
	/// returns the address of the texture
	///
	GLuint getTexture(const std::string & _name);

	/// 
	/// loadCubemap:
	/// Loads a cubemap with an associated name
	/// _name - the name for the cubemap
	/// _path - a vector containing the paths to the cubemap files
	/// _alphaEnabled - whether the textures contain alpha channels
	/// returns the address of the cubemap
	///
	GLuint loadCubeMap(const std::string & _name, const std::vector<std::string> & _paths, const bool _alphaEnabled = false);

	/// 
	/// getCubemap:
	/// Gets a cubemap associated with a given name
	/// _name - the name for the cubemap
	/// returns the address of the cubemap
	///
	GLuint getCubeMap(const std::string & _name);

	/// 
	/// loadSound:
	/// Loads a sound with an associated name
	/// _name - the name for the sound
	/// _path - the path to the sound file
	/// _mode - the FMOD_MODE for the sound
	/// returns a pointer to the sound
	///
	FMOD::Sound * loadSound(const std::string & _name, const std::string & _path, const FMOD_MODE & _mode = FMOD_INIT_NORMAL);

	/// 
	/// getSound:
	/// Gets a sound associated with a given name
	/// _name - the name for the sound
	/// returns a pointer to the sound
	///
	FMOD::Sound * getSound(const std::string & _name);

	/// 
	/// playSound:
	/// Plays a sound
	/// _sound - a pointer to the sound
	/// returns the channel on which the sound is playing
	///
	FMOD::Channel * playSound(FMOD::Sound * _sound);

	/// 
	/// initSound:
	/// Initialises FMOD
	///
	bool initSound();

	/// 
	/// update:
	/// Updates FMOD
	///
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

