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
#include "GameWorld.h"

#include "JNetwork\Client.h"
#include "JNetwork\Util.h"


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
	continueText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f, 2.f * Game::getGame()->getWindowHeight() / 5.f));

	if (Input::isKeyDown('f'))
	{
		Game::getGame()->createGameWorld();
		SceneManager::getSceneManager().activate<ScenePlay>();
	}

	if (Input::isKeyDown('g'))
	{
		SceneManager::getSceneManager().activate<ScenePlay>();
	}
}

void SceneMenu::reset()
{
//	//TODO: Change to actual menu....
//	std::cout << "Client or server? " << std::endl;
//	int input;
//	std::cin >> input;
//
//	Game::getGame()->createGameWorld(input == 1);
//
//	Logger::getLogger().log("Starting network entity");
//	Game::getGame()->getGameWorld().startNetwork();
//
//	if (!Game::getGame()->getGameWorld().getIsServer())
//	{
//		auto _client = dynamic_cast<JNetwork::Client *>(Game::getGame()->getGameWorld().getNetworkEntity());
//
//
//
//
//		_client->broadcastForServers(5000);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//		
//		auto serverList = _client->getBroadcastFoundServers();
//		std::cout << "Found " << serverList.size() << " server" << (serverList.size() > 1 ? "s" : "") << ": " << std::endl;
//		for (unsigned int i = 0; i < serverList.size(); ++i)
//		{
//			std::cout << i + 1 << ": " << JNetwork::addrToString(serverList[i]) << std::endl;
//		}
//		
//		std::cout << "Please enter the number corresponding to the server which you wish to join";
//					
//		int chosen;
//		std::cin >> chosen;
//		--chosen;
//		
//		auto serverAddr = serverList[chosen];
//
//
//
//
//		if (Game::getGame()->getGameWorld().clientConnect(serverAddr, "Jack", 5000))
//		{
//			Logger::getLogger().log("Connected to server successfully");
//		}
//	}
}
