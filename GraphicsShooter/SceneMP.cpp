#include "SceneMP.h"

#include "Game.h"
#include "Logger.h"
#include "GameWorld.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "SceneMenu.h"

#include "JNetwork\Client.h"
#include "JNetwork\Server.h"
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

	joinServerMenu->clear();

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

		std::cout << i + 1 << ": " << JNetwork::addrToString(serverList[i].first) << std::endl;

		//Display servers as buttons
		joinServerMenu->addElement(new UIElement(
			0.75f,
			1.f - ((i + 1) / (serverList.size() + 1)), 
			addrCopy.second,
			true,

			[addrCopy, this]() {
			if (Game::getGame()->getGameWorld().clientConnect(addrCopy.first, this->clientUsername, 1000))
			{
				Logger::getLogger().log("Connected to server successfully");
				SceneManager::getSceneManager().activate<ScenePlay>();
			}
		}));
	}

	if (serverList.size() == 0)
	{
		joinServerMenu->addElement(new UIElement(0.75f, 0.5f, "No servers found", false));
	}

	joinServerMenu->addElement(new UIElement(0.25f, 0.5f, "Back", true, [&]() {
		this->backToMPMenu();
	}));
}

void SceneMP::setupHostServer()
{
	this->mainMenuShown = false;
	this->hostServerMenuShown = true;

	std::string serverName;

	std::cout << "Enter a server name: " << std::endl;
	std::getline(std::cin, serverName);

	std::cout << "Enter a username: " << std::endl;
	std::getline(std::cin, clientUsername);

	Game::getGame()->createGameWorld(true, true, clientUsername, serverName);

	Logger::getLogger().log("Starting network entity");
	Game::getGame()->getGameWorld().startNetwork();
}

void SceneMP::backToMPMenu()
{
	mainMenuShown = true;
	joinServerMenuShown = false;
	hostServerMenuShown = false;
}

void SceneMP::reset()
{
	mainMenu->clear();
	hostServerMenu->clear();
	joinServerMenu->clear();

	backToMPMenu();

	mainMenu->addElement(new UIElement(0.5f, 4.f / 5.f, "Join Server", true, [this]() {
		this->setupJoinServer();
	}));

	mainMenu->addElement(new UIElement(0.5f, 3.f / 5.f, "Create Server", true, [this]() {
		this->setupHostServer();
	}));

	mainMenu->addElement(new UIElement(0.5f, 2.f / 5.f, "Back", true, [this]() {
		SceneManager::getSceneManager().activate<SceneMenu>();
	}));
	

	hostServerMenu->addElement(new UIElement(0.5f, 3.f / 5.f, "Start the Game!", true, [this]() {
		dynamic_cast<JNetwork::Server *>(Game::getGame()->getGameWorld().getNetworkEntity())->setDenyConnections(true);
		SceneManager::getSceneManager().activate<ScenePlay>();
	}));
}
