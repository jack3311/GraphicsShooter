//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneMP.h
//  Description  :    The multiplayer menu scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Scene.h"

#include "UIMenu.h"

class SceneMP :
	public Scene
{
private:
	UIMenu * mainMenu;
	bool mainMenuShown = true;

	UIMenu * joinServerMenu;
	bool joinServerMenuShown = false;

	UIMenu * hostServerMenu;
	bool hostServerMenuShown = false;

	std::string clientUsername = "";

public:
	SceneMP();
	virtual ~SceneMP();

	virtual void render() const;
	virtual void update(float _dt);

	/// 
	/// setupJoinServer:
	/// Sets up the join server menu
	///
	void setupJoinServer();

	/// 
	/// setupHostServer:
	/// Sets up the host server menu
	///
	void setupHostServer();

	/// 
	/// backToMPMenu:
	/// Sets up the main multiplayer menu
	///
	void backToMPMenu();

	/// 
	/// reset:
	/// Resets the scene
	///
	void reset();
};

