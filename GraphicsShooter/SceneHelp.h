//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneHelp.h
//  Description  :    The help scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Scene.h"

#include "TextLabel.h"
#include "UIMenu.h"

class SceneHelp :
	public Scene
{
private:
	TextLabel * titleText;
	TextLabel * helpText;

	UIMenu * helpMenu;

public:
	SceneHelp();
	~SceneHelp();

	virtual void render() const;
	virtual void update(float _dt);

	/// 
	/// reset:
	/// Resets the scene
	///
	void reset();
};

