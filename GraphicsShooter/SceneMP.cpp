#include "SceneMP.h"

#include "Game.h"
#include "Logger.h"
#include "GameWorld.h"

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
	if (mainMenuShown)
		mainMenu->update(_dt);

	if (joinServerMenuShown)
		joinServerMenu->update(_dt);

	if (hostServerMenuShown)
		hostServerMenu->update(_dt);
}

void SceneMP::setupJoinServer()
{
	mainMenuShown = false;

	Game::getGame()->createGameWorld(false, true);

	Logger::getLogger().log("Starting network entity");
	Game::getGame()->getGameWorld().startNetwork();

	std::cout << "Enter a username: " << std::endl;
	std::cin >> clientUsername;

	auto _client = dynamic_cast<JNetwork::Client *>(Game::getGame()->getGameWorld().getNetworkEntity());

	_client->broadcastForServers(5000);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	auto serverList = _client->getBroadcastFoundServers();
	std::cout << "Found " << serverList.size() << " server" << (serverList.size() > 1 ? "s" : "") << ": " << std::endl;
	for (unsigned int i = 0; i < serverList.size(); ++i)
	{
		std::cout << i + 1 << ": " << JNetwork::addrToString(serverList[i]) << std::endl;

		joinServerMenu->addElement(new UIElement(0.75f, i / serverList.size(), "Join Server", true, [=]() {

		}));
	}

}

void SceneMP::reset()
{
	mainMenu->clear();

	mainMenu->addElement(new UIElement(0.5f, 4.f / 5.f, "Join Server", true, [=]() {
	}));

	mainMenu->addElement(new UIElement(0.5f, 3.f / 5.f, "Create Server", true, [=]() {
		mainMenuShown = false;
		Game::getGame()->createGameWorld(true, true);
		Logger::getLogger().log("Starting network entity");
		Game::getGame()->getGameWorld().startNetwork();
	}));
}
