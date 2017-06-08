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

	gameOverMenu->addElement(new UIElement(0.5f, 3.f / 4.f, "Game Over"));

	std::stringstream ss;
	ss << "Your score was: " << _playerScore;

	gameOverMenu->addElement(new UIElement(0.5f, 2.f / 4.f, ss.str()));

	gameOverMenu->addElement(new UIElement(0.5f, 1.f / 4.f, "Return to main menu", []() {
		SceneManager::getSceneManager().activate<SceneMenu>();
	}));
}