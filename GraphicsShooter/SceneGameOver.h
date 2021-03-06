//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneGameOver.h
//  Description  :    The game over scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Scene.h"
#include "UIMenu.h"

class SceneGameOver :
	public Scene
{
private:
	UIMenu * gameOverMenu;

public:
	SceneGameOver();
	virtual ~SceneGameOver();

	virtual void render() const;
	virtual void update(float _dt);

	/// 
	/// reset:
	/// Resets the scene
	/// _playerScore - the player score to show
	///
	void reset(int _playerScore);
};

