//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneGameOver.cpp
//  Description  :    The game over scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "SceneGameOver.h"

#include <sstream>

#include "SceneManager.h"
#include "SceneMenu.h"



SceneGameOver::SceneGameOver()
{
	gameOverMenu = new UIMenu();
	gameOverMenu->initialise();
}


SceneGameOver::~SceneGameOver()
{
	delete gameOverMenu;
}

void SceneGameOver::render() const
{
	gameOverMenu->draw();
}

void SceneGameOver::update(float _dt)
{
	gameOverMenu->update(_dt);
}

void SceneGameOver::reset(int _playerScore)
{
	gameOverMenu->clear();

	gameOverMenu->addElement(new UIElement(0.5f, 3.f / 4.f, "Game Over", false));

	std::stringstream ss;
	ss << "Your score was: " << _playerScore;

	gameOverMenu->addElement(new UIElement(0.5f, 2.f / 4.f, ss.str(), false));

	gameOverMenu->addElement(new UIElement(0.5f, 1.f / 4.f, "Return to main menu", true, []() {
		SceneManager::getSceneManager().activate<SceneMenu>();
	}));
}