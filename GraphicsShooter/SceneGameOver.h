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

	void reset(int _playerScore);
};

