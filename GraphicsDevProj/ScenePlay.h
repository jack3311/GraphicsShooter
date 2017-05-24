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

class ScenePlay :
	public Scene
{
private:
	float elapsedTime = 0.f;

	ModelRenderer * modelRendererTest;
	SkyboxRenderer * skyboxRenderer;

	std::vector<Light *> lights;
	int numLights;

	float ambientStrength;
	glm::vec3 ambientColor;
	float specularStrength;

public:
	ScenePlay();
	virtual ~ScenePlay();

	virtual void render() const;
	virtual void update(float _dt);

	void reset();
};

