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

	void setupJoinServer();
	void setupHostServer();
	void backToMPMenu();

	/// 
	/// reset:
	/// Resets the scene
	///
	void reset();
};

