//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    ScenePlay.h
//  Description  :    The play scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\FMOD\fmod.hpp"

#include "Scene.h"
#include "Sprite.h"
#include "TextLabel.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"

class Object;

class ScenePlay :
	public Scene
{
private:
	float elapsedTime = 0.f;

	MeshRenderer * floorRenderer;

	ModelRenderer * playerRenderer;
	ModelRenderer ** enemyRenderers;

	MeshRenderer * bulletRenderer;

	MeshRenderer ** powerupRenderers;

	SkyboxRenderer * skyboxRenderer;

	std::vector<Light *> lights;

	float ambientStrength;
	glm::vec3 ambientColor;
	float specularStrength;

	bool spaceBarLastFrame = false;

	TextLabel * ammoText;
	TextLabel * scoreText;
	TextLabel * healthText;

public:
	ScenePlay();
	virtual ~ScenePlay();

	virtual void render() const;
	virtual void update(float _dt);

	/// 
	/// reset:
	/// Resets the scene
	///
	void reset();

	/// 
	/// getLights:
	/// Returns the vector of lights in the scene
	///
	const std::vector<Light *> & getLights() const;

	/// 
	/// getNumLights:
	/// Returns the number of lights in the scene
	///
	const unsigned int getNumLights() const;

	/// 
	/// getAmbientStrength:
	/// Returns the strength of ambient lighting
	///
	const float getAmbientStrength() const;

	/// 
	/// getAmbientColor:
	/// Returns the color of the ambient light
	///
	const glm::vec3 & getAmbientColor() const;

	/// 
	/// getSpecularStrength:
	/// Returns the specular strength of the scene
	///
	const float getSpecularStrength() const;

	/// 
	/// getSkyboxCubemap:
	/// Returns the address of the current skybox
	///
	const GLuint getSkyboxCubemap() const;
};

