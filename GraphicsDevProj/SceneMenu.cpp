//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SceneMenu.cpp
//  Description  :    The menu scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "SceneMenu.h"

#include "AssetManager.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "Input.h"
#include "Game.h"



SceneMenu::SceneMenu()
{
	continueText = new TextLabel("Press space to continue. . .", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), true);
	continueText->setScale(0.75f);
}

SceneMenu::~SceneMenu()
{
	delete continueText;
}

void SceneMenu::render() const
{
	continueText->Render();
}

void SceneMenu::update(float _dt)
{
	SceneManager::getSceneManager().getScene<ScenePlay>().update(0.f);

	continueText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 2.f * Game::getGame()->getWindowHeight() / 5.f));

	if (Input::isKeyDown(' '))
	{
		SceneManager::getSceneManager().activateWithoutReset<ScenePlay>();
	}
}

void SceneMenu::reset()
{
}
