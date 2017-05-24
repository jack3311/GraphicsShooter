//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    AssetManager.cpp
//  Description  :    Manages all program assets
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "AssetManager.h"

AssetManager * AssetManager::assetManager = nullptr;

AssetManager & AssetManager::getAssetManager()
{
	if (!AssetManager::assetManager)
	{
		assetManager = new AssetManager();
	}

	return *assetManager;
}

void AssetManager::deleteAssetManager()
{
	delete assetManager;
}

GLuint AssetManager::loadTextureSimple(const std::string & _path, const bool _alphaEnabled)
{
	//Load texture
	GLuint texture;

	int width, height;
	unsigned char * image = SOIL_load_image(_path.c_str(), &width, &height, 0, _alphaEnabled ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, _alphaEnabled ? GL_RGBA : GL_RGB, width, height, 0, _alphaEnabled ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);

	//Add texture to map
	return texture;
}

GLuint AssetManager::loadTexture(const std::string & _name, const std::string & _path, const bool _alphaEnabled)
{
	//Load texture
	GLuint texture = loadTextureSimple(_path, _alphaEnabled);

	//Add texture to map
	textureMap[_name] = texture;
	return texture;
}

GLuint AssetManager::getTexture(const std::string & _name)
{
	return textureMap[_name];
}

GLuint AssetManager::loadCubeMap(const std::string & _name, const std::vector<std::string> & _paths, const bool _alphaEnabled)
{
	//Create cube map
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	//Assign textures to cube map
	int width, height;
	unsigned char * image = nullptr;
	for (unsigned int i = 0; i < _paths.size(); ++i)
	{
		image = SOIL_load_image(_paths[i].c_str(), &width, &height, 0, _alphaEnabled ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _alphaEnabled ? GL_RGBA : GL_RGB, width, height, 0, _alphaEnabled ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	//Set tex parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Add cube map to map
	cubeMapMap[_name] = texture;
	return texture;
}

GLuint AssetManager::getCubeMap(const std::string & _name)
{
	return cubeMapMap[_name];
}

FMOD::Sound * AssetManager::loadSound(const std::string & _name, const std::string & _path, const FMOD_MODE & _mode)
{
	FMOD_RESULT result;

	FMOD::Sound* sound = nullptr;

	result = audioManager->createSound(_path.c_str(), FMOD_DEFAULT, 0, &sound);

	if (result != FMOD_OK) return nullptr;

	result = sound->setMode(_mode);

	if (result != FMOD_OK) return nullptr;

	soundMap[_name] = sound;

	return sound;
}

FMOD::Sound * AssetManager::getSound(const std::string & _name)
{
	return soundMap[_name];
}

FMOD::Channel * AssetManager::playSound(FMOD::Sound * _sound)
{
	FMOD::Channel * outChannel = nullptr;
	audioManager->playSound(_sound, 0, false, &outChannel); //4th is 'out' parameter
	return outChannel; //Return copy of pointer
}

bool AssetManager::initSound()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&audioManager);
	if (result != FMOD_OK) return false;

	result = audioManager->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) return false;

	return true;
}

void AssetManager::update()
{
	audioManager->update();
}

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	for (std::pair<std::string, GLuint> texture : textureMap)
	{
		glDeleteTextures(1, &texture.second);
	}

	textureMap.clear();

	for (std::pair<std::string, FMOD::Sound *> sound : soundMap)
	{
		sound.second->release();
	}

	soundMap.clear();

	audioManager->release();
}