#include "GameWorld.h"

#include "JNetwork\Server.h"
#include "JNetwork\Client.h"

#include "Logger.h"


GameWorld::GameWorld(/*bool _isServer*/)/* : isServer(_isServer)*/
{
	//if (_isServer)
	//{
	//	//Use intermediary lambda function to use 'this' to call member function
	//	networkEntity = new JNetwork::Server([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
	//		this->onReceivePacket(_p, _a);
	//	});
	//}
	//else
	//{
	//	//Use intermediary lambda function to use 'this' to call member function
	//	networkEntity = new JNetwork::Client([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
	//		this->onReceivePacket(_p, _a);
	//	});
	//}

	player = new Object(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	for (int i = 0; i < 25; ++i)
	{
		enemies.push_back(new Object(glm::vec3(rand() % 201 - 100, 0, rand() % 201 - 100), glm::vec3(0.1, 0.1, 0.1)));
	}
}

GameWorld::~GameWorld()
{
	for (auto obj : enemies)
	{
		delete obj;
	}
}

void GameWorld::update(float _dt)
{
	timeSinceReloadStart += _dt;

	//Update bullets
	for (auto bullet : bullets)
	{
		bullet->update(_dt);
	}

	//Enemy AI:
	for (auto enemy : enemies)
	{
		//Path towards player

		glm::vec3 delta = player->getPosition() - enemy->getPosition();
		glm::vec3 delScl = glm::normalize(delta) * ENEMY1_MAX_SPEED * _dt;
		
		
		if (length2(enemy->getPosition() + delScl - player->getPosition()) < 9)
		{
			enemy->setPosition(player->getPosition() + glm::normalize(enemy->getPosition() - player->getPosition()) * 3.f);
		
			//TODO: Damage player
		}
		else
		{
			enemy->move(delScl);
		}






		for (auto enemy1 : enemies)
		{
			if (enemy1 != enemy)
			{
				//Stay away from other enemies
				glm::vec3 delta2 = enemy->getPosition() - enemy1->getPosition();
				glm::vec3 delScl2 = glm::normalize(delta2) * ENEMY1_MAX_SPEED * _dt * (1.f / length2(delta2));

				enemy->move(delScl2);
			}
		}

	}


	//if (isServer)
	//{
	//	//Update position of test object TODO: Remove
	//	testObject->move(glm::vec3(1, 0, -1) * _dt);

	//	//Send positions to all clients

	//	char data[sizeof(glm::vec3) + 1];
	//	data[0] = 5;
	//	//toData(&data[1], &testObject->getPosition());

	//	floatToChar(testObject->getPosition().x, &data[1]);
	//	floatToChar(testObject->getPosition().y, &data[1 + sizeof(float)]);
	//	floatToChar(testObject->getPosition().z, &data[1 + 2 * sizeof(float)]);

	//	dynamic_cast<JNetwork::Server *>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, data));
	//}
	//else
	//{
	//	//Parse packet pool
	//	while (clientPacketQueue.size() > 0)
	//	{
	//		clientPacketQueueMutex.lock();
	//		auto packetPair = clientPacketQueue.front();
	//		clientPacketQueue.pop();
	//		clientPacketQueueMutex.unlock();

	//		parsePacket(packetPair.first, packetPair.second);
	//	}
	//}
}

std::vector<Object*> GameWorld::getEnemies()
{
	return enemies;
}

std::vector<PhysicsObject*> GameWorld::getBullets()
{
	return bullets;
}

Object * GameWorld::getPlayer()
{
	return player;
}

void GameWorld::playerFire()
{
	//Check if reload is finished
	if (timeSinceReloadStart >= RELOAD_TIME && playerAmmo == 0)
	{
		playerAmmo = CLIP_SIZE;
	}

	if (playerAmmo > 0)
	{
		auto facing2D = -glm::vec3(player->getRotation() * glm::vec4(0, 0, 1.f, 0.f));

		PhysicsObject * bullet = new PhysicsObject(player->getPosition(), glm::vec3(0.4, 0.4, 0.4));
		bullet->move(facing2D * 0.5f + glm::vec3(0, 1, 0));
		bullet->setVelocity(facing2D * BULLET_MAX_SPEED);

		bullets.push_back(bullet);


		//Start reload if this is last bullet
		if (playerAmmo == 1)
		{
			timeSinceReloadStart = 0.f;
		}
		
		//Reduce ammo
		--playerAmmo;
	}
}

//void GameWorld::onReceivePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr)
//{
//	if (isServer)
//	{
//		//Directly parse packet (this is running on a different thread)
//		parsePacket(_p, _addr);
//	}
//	else
//	{
//		//Pool packet for next update
//		clientPacketQueueMutex.lock();
//		clientPacketQueue.push(std::pair<JNetwork::JNetworkPacket, sockaddr_in>(JNetwork::JNetworkPacket(_p), _addr));
//		clientPacketQueueMutex.unlock();
//	}
//}
//
//void GameWorld::parsePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr)
//{
//	if (isServer)
//	{
//		//Server
//
//	}
//	else
//	{
//		//Client
//		if (_p.data[1] == 5)
//		{
//			glm::vec3 position;
//
//			position.x = charToFloat(&_p.data[2]);
//			position.y = charToFloat(&_p.data[2 + sizeof(float)]);
//			position.z = charToFloat(&_p.data[2 + 2 * sizeof(float)]);
//
//			testObject->setPosition(position);
//		}
//	}
//}
//
//void GameWorld::startNetwork()
//{
//	networkEntity->start();
//	networkEntity->initialise(5000);
//}
//
//void GameWorld::stopNetwork()
//{
//	networkEntity->stop();
//}
//
//bool GameWorld::getIsServer() const
//{
//	return isServer;
//}
//
//bool GameWorld::clientConnect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout)
//{
//	return dynamic_cast<JNetwork::Client *>(networkEntity)->connect(_serverAddr, _clientName, _timeout);
//}
//
//JNetwork::INetworkEntity * GameWorld::getNetworkEntity()
//{
//	return networkEntity;
//}
