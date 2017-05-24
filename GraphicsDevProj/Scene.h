//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Scene.h
//  Description  :    An interface for game scenes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Camera.h"
#include "SpriteRenderer.h"

class Scene
{
public:
	Scene();
	~Scene();

	virtual void render() const = 0;
	virtual void update(float _dt) = 0;
};

