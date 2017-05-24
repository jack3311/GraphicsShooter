//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneMenu.h
//  Description  :    The menu scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "TextLabel.h"

class SceneMenu :
	public Scene
{
private:
	float timeAccum = 0.f;
	TextLabel * continueText;

public:
	SceneMenu();
	virtual ~SceneMenu();

	virtual void render() const;
	virtual void update(float _dt);

	void reset();
};