#include "SceneMP.h"

#include "Game.h"
#include "Logger.h"
#include "GameWorld.h"
#include "SceneManager.h"
#include "ScenePlay.h"

#include "JNetwork\Client.h"
#include "JNetwork\Util.h"



SceneMP::SceneMP()
{
	mainMenu = new UIMenu();
	mainMenu->initialise();

	joinServerMenu = new UIMenu();
	joinServerMenu->initialise();

	hostServerMenu = new UIMenu();
	hostServerMenu->initialise();
}


SceneMP::~SceneMP()
{
	delete mainMenu;
}

void SceneMP::render() const
{
	if (mainMenuShown)
		mainMenu->draw();

	if (joinServerMenuShown)
		joinServerMenu->draw();

	if (hostServerMenuShown)
		hostServerMenu->draw();
}

void SceneMP::update(float _dt)
{
	if (joinServerMenuShown)
		joinServerMenu->update(_dt);

	if (hostServerMenuShown)
		hostServerMenu->update(_dt);

	if (mainMenuShown)
		mainMenu->update(_dt);
}

void SceneMP::setupJoinServer()
{
	mainMenuShown = false;
	joinServerMenuShown = true;

	std::cout << "Enter a username: " << std::endl;
	std::cin >> clientUsername;

	Game::getGame()->createGameWorld(false, true, clientUsername);

	Logger::getLogger().log("Starting network entity");
	Game::getGame()->getGameWorld().startNetwork();

	auto _client = dynamic_cast<JNetwork::Client *>(Game::getGame()->getGameWorld().getNetworkEntity());

	//Broadcast for servers

	_client->broadcastForServers(5000);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	auto & serverList = _client->getBroadcastFoundServers();
	std::cout << "Found " << serverList.size() << " server" << (serverList.size() > 1 ? "s" : "") << ": " << std::endl;
	for (unsigned int i = 0; i < serverList.size(); ++i)
	{
		auto addrCopy = serverList[i];

		std::cout << i + 1 << ": " << JNetwork::addrToString(serverList[i]) << std::endl;

		//Display servers as buttons
		joinServerMenu->addElement(new UIElement(0.5f, i / (serverList.size() + 1), JNetwork::addrToString(addrCopy), true, [addrCopy, this]() {
			if (Game::getGame()->getGameWorld().clientConnect(addrCopy, this->clientUsername, 5000))
			{
				Logger::getLogger().log("Connected to server successfully");
				SceneManager::getSceneManager().activate<ScenePlay>();
			}
		}));
	}
}

void SceneMP::setupHostServer()
{
	this->mainMenuShown = false;
	this->hostServerMenuShown = true;

	std::cout << "Enter a username: " << std::endl;
	std::cin >> clientUsername;

	Game::getGame()->createGameWorld(true, true, clientUsername);

	Logger::getLogger().log("Starting network entity");
	Game::getGame()->getGameWorld().startNetwork();
}

void SceneMP::reset()
{
	mainMenu->clear();
	hostServerMenu->clear();
	joinServerMenu->clear();

	mainMenu->addElement(new UIElement(0.5f, 4.f / 5.f, "Join Server", true, [this]() {
		this->setupJoinServer();
	}));

	mainMenu->addElement(new UIElement(0.5f, 3.f / 5.f, "Create Server", true, [this]() {
		this->setupHostServer();
	}));
	

	hostServerMenu->addElement(new UIElement(0.5f, 3.f / 5.f, "Start the Game!", true, [this]() {
		SceneManager::getSceneManager().activate<ScenePlay>();
	}));
}
