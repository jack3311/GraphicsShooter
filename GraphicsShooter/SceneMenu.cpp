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

#include <functional>

#include "AssetManager.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "Input.h"
#include "Game.h"
#include "GameWorld.h"
#include "SceneMP.h"

#include "JNetwork\Client.h"
#include "JNetwork\Util.h"


SceneMenu::SceneMenu()
{
	mainMenu = new UIMenu();
	mainMenu->initialise();

	//Start Bg music - the constructor is called only once
	Logger::getLogger().log("Starting background music");
	//AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("bgMusic")); //TODO: REENABLE THIS
}

SceneMenu::~SceneMenu()
{
	delete mainMenu;
}

void SceneMenu::render() const
{
	mainMenu->draw();
}

void SceneMenu::update(float _dt)
{
	mainMenu->update(_dt);
}

void SceneMenu::reset()
{
	mainMenu->clear();

	mainMenu->addElement(new UIElement(0.5f, 5.f / 6.f, "Tank Survival", false));
	
	if (Game::getGame()->hasGameWorld() && Game::getGame()->getGameWorld().isGameInProgress())
	{
		if (Game::getGame()->getGameWorld().getIsMultiplayer())
		{
			if (Game::getGame()->getGameWorld().getIsServer())
			{
				mainMenu->addElement(new UIElement(0.5f, 4.f / 6.f, "Continue", true, []() {
					SceneManager::getSceneManager().activate<ScenePlay>();
				}));

				mainMenu->addElement(new UIElement(0.5f, 3.f / 6.f, "Stop Server", true, []() {
					Game::getGame()->deleteGameWorld();
					SceneManager::getSceneManager().activate<SceneMenu>();
				}));
			}
			else if (dynamic_cast<JNetwork::Client *>(Game::getGame()->getGameWorld().getNetworkEntity())->isConnectedToServer())
			{
				mainMenu->addElement(new UIElement(0.5f, 4.f / 6.f, "Continue", true, []() {
					SceneManager::getSceneManager().activate<ScenePlay>();
				}));

				mainMenu->addElement(new UIElement(0.5f, 3.f / 6.f, "Disconnect", true, []() {
					Game::getGame()->deleteGameWorld();
					SceneManager::getSceneManager().activate<SceneMenu>();
				}));
			}
		}
		else
		{
			mainMenu->addElement(new UIElement(0.5f, 4.f / 6.f, "Continue", true, []() {
				SceneManager::getSceneManager().activate<ScenePlay>();
			}));
		}
	}

	mainMenu->addElement(new UIElement(0.5f, 2.f / 6.f, "New Single Player Game", true, []() {
		Game::getGame()->createGameWorld(true, false, "You");
		SceneManager::getSceneManager().activate<ScenePlay>();
	}));

	mainMenu->addElement(new UIElement(0.5f, 1.f / 6.f, "New Multi Player Game", true, []() {
		SceneManager::getSceneManager().activate<SceneMP>();
	}));






	////TODO: Change to actual menu....
	//std::cout << "Client or server? " << std::endl;
	//int input;
	//std::cin >> input;

	//Game::getGame()->createGameWorld(input == 1);

	//Logger::getLogger().log("Starting network entity");
	//Game::getGame()->getGameWorld().startNetwork();

	//if (!Game::getGame()->getGameWorld().getIsServer())
	//{
	//	auto _client = dynamic_cast<JNetwork::Client *>(Game::getGame()->getGameWorld().getNetworkEntity());




	//	_client->broadcastForServers(5000);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	
	//	auto serverList = _client->getBroadcastFoundServers();
	//	std::cout << "Found " << serverList.size() << " server" << (serverList.size() > 1 ? "s" : "") << ": " << std::endl;
	//	for (unsigned int i = 0; i < serverList.size(); ++i)
	//	{
	//		std::cout << i + 1 << ": " << JNetwork::addrToString(serverList[i]) << std::endl;
	//	}
	//	
	//	std::cout << "Please enter the number corresponding to the server which you wish to join";
	//				
	//	int chosen;
	//	std::cin >> chosen;
	//	--chosen;
	//	
	//	auto serverAddr = serverList[chosen];




	//	if (Game::getGame()->getGameWorld().clientConnect(serverAddr, "Jack", 5000))
	//	{
	//		Logger::getLogger().log("Connected to server successfully");
	//	}
	//}
}
