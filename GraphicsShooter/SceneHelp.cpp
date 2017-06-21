//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneHelp.cpp
//  Description  :    The help scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "SceneHelp.h"

#include "SceneMenu.h"
#include "SceneManager.h"
#include "Game.h"



SceneHelp::SceneHelp()
{
	titleText = new TextLabel("Help", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), true);
	helpText = new TextLabel("", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), true);
	helpText->setScale(0.5f);

	helpMenu = new UIMenu();
	helpMenu->initialise();
	helpMenu->addElement(new UIElement(0.5f, 1.f / 8.f, "Back", true, []() {
		SceneManager::getSceneManager().activate<SceneMenu>();
	}));
}


SceneHelp::~SceneHelp()
{
	delete titleText;
	delete helpText;
	delete helpMenu;
}

void SceneHelp::render() const
{
	titleText->Render();

	helpText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 6.f / 8.f * Game::getGame()->getWindowHeight()));
	helpText->setText("Use WASD to move.");
	helpText->Render();

	helpText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 5.f / 8.f * Game::getGame()->getWindowHeight()));
	helpText->setText("Use the mouse to navigate menus.");
	helpText->Render();

	helpText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 4.f / 8.f * Game::getGame()->getWindowHeight()));
	helpText->setText("Press ESC to pause.");
	helpText->Render();

	helpText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 3.f / 8.f * Game::getGame()->getWindowHeight()));
	helpText->setText("Press Space to fire");
	helpText->Render();

	helpText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 2.f / 8.f * Game::getGame()->getWindowHeight()));
	helpText->setText("Press TAB to see player status");
	helpText->Render();

	helpMenu->draw();
}

void SceneHelp::update(float _dt)
{
	titleText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 7.f / 8.f * Game::getGame()->getWindowHeight()));

	helpMenu->update(_dt);
}

void SceneHelp::reset()
{
}
