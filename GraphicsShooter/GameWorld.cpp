#include "GameWorld.h"

#include "JNetwork\Server.h"
#include "JNetwork\Client.h"

#include "Logger.h"


GameWorld::GameWorld(bool _isServer) : isServer(_isServer)
{
	if (_isServer)
	{
		//Use intermediary lambda function to use 'this' to call member function
		networkEntity = new JNetwork::Server([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
			this->onReceivePacket(_p, _a);
		});
	}
	else
	{
		//Use intermediary lambda function to use 'this' to call member function
		networkEntity = new JNetwork::Client([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
			this->onReceivePacket(_p, _a);
		});
	}

	testObject = new Object(glm::vec3(5, 0, 5), glm::vec3(1, 1, 1));
}

GameWorld::~GameWorld()
{
}

void GameWorld::update(float _dt)
{
	if (isServer)
	{
		//Update position of test object TODO: Remove
		testObject->move(glm::vec3(1, 0, -1) * _dt);

		//Send positions to all clients

		char data[sizeof(glm::vec3) + 1];
		data[0] = 5;
		//toData(&data[1], &testObject->getPosition());

		floatToChar(testObject->getPosition().x, &data[1]);
		floatToChar(testObject->getPosition().y, &data[1 + sizeof(float)]);
		floatToChar(testObject->getPosition().z, &data[1 + 2 * sizeof(float)]);

		dynamic_cast<JNetwork::Server *>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, data));
	}
	else
	{
		//Parse packet pool
		while (clientPacketQueue.size() > 0)
		{
			clientPacketQueueMutex.lock();
			auto packetPair = clientPacketQueue.front();
			clientPacketQueue.pop();
			clientPacketQueueMutex.unlock();

			parsePacket(packetPair.first, packetPair.second);
		}
	}
}

void GameWorld::onReceivePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr)
{
	if (isServer)
	{
		//Directly parse packet (this is running on a different thread)
		parsePacket(_p, _addr);
	}
	else
	{
		//Pool packet for next update
		clientPacketQueueMutex.lock();
		clientPacketQueue.push(std::pair<JNetwork::JNetworkPacket, sockaddr_in>(JNetwork::JNetworkPacket(_p), _addr));
		clientPacketQueueMutex.unlock();
	}
}

void GameWorld::parsePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr)
{
	if (isServer)
	{
		//Server

	}
	else
	{
		//Client
		if (_p.data[1] == 5)
		{
			glm::vec3 position;

			position.x = charToFloat(&_p.data[2]);
			position.y = charToFloat(&_p.data[2 + sizeof(float)]);
			position.z = charToFloat(&_p.data[2 + 2 * sizeof(float)]);

			testObject->setPosition(position);
		}
	}
}

void GameWorld::startNetwork()
{
	networkEntity->start();
	networkEntity->initialise(5000);
}

void GameWorld::stopNetwork()
{
	networkEntity->stop();
}

bool GameWorld::getIsServer() const
{
	return isServer;
}

bool GameWorld::clientConnect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout)
{
	return dynamic_cast<JNetwork::Client *>(networkEntity)->connect(_serverAddr, _clientName, _timeout);
}

JNetwork::INetworkEntity * GameWorld::getNetworkEntity()
{
	return networkEntity;
}
