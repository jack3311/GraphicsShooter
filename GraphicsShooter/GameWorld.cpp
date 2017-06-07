#include "GameWorld.h"

#include "JNetwork\Server.h"
#include "JNetwork\Client.h"

#include "Logger.h"
#include "SceneManager.h"
#include "SceneMenu.h"


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

	player = new Player(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	for (int i = 0; i < 25; ++i)
	{
		enemies.push_back(new Object(glm::vec3(rand() % 201 - 100, 0, rand() % 201 - 100), glm::vec3(4.f / 40.f, 4.f / 40.f, 4.f / 40.f)));
	}

	gameInProgress = true;
}

GameWorld::~GameWorld()
{
	for (auto obj : enemies)
	{
		delete obj;
	}

	//TODO: Stop network entity
}

void GameWorld::update(float _dt)
{
	//Check for player death
	if (player->isDead())
	{
		gameInProgress = false;
		SceneManager::getSceneManager().activate<SceneMenu>();
	}



	//Update player:
	player->update(_dt);

	//Update enemies:
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

		//Face towards player
		enemy->lookAt(player->getPosition());

		//Try to shoot
		if (rand() % 10000 < 1000 * _dt)
		{
			if (enemy->getPosition() != player->getPosition())
			{
				auto facing2D = normalize(player->getPosition() - enemy->getPosition());

				createBullet(enemy->getPosition() + facing2D * 5.f + glm::vec3(0.f, (36.f / 40.f) * 4.f, 0.f), facing2D, false);
			}
		}


		//Stay away from other enemies
		for (auto enemy1 : enemies)
		{
			if (enemy1 != enemy)
			{
				//Move away from other enemy based on the distance between them
				glm::vec3 delta2 = enemy->getPosition() - enemy1->getPosition();
				glm::vec3 delScl2 = glm::normalize(delta2) * ENEMY1_MAX_SPEED * _dt * (1.f / length2(delta2));

				enemy->move(delScl2);
			}
		}

	}

	//Update bullets
	for (auto itr = bullets.begin(); itr != bullets.end(); )
	{
		(*itr)->update(_dt);

		bool collision = false;
		
		//Check for collision with enemies
		if ((*itr)->flag != 0) //If bullet is friendly
		{
			for (auto itr2 = enemies.begin(); itr2 != enemies.end(); )
			{
				//Check collision with two spheres
				if (sphereSphereCollision((*itr2)->getPosition(), ENEMY_COLLISION_RADIUS, (*itr)->getPosition(), BULLET_COLLISION_RADIUS))
				{
					itr2 = enemies.erase(itr2);
					collision = true;
					break;
				}
				else
				{
					++itr2;
				}
			}
		}

		//Check for collision with player
		if ((*itr)->flag == 0) //If bullet is not friendly
		{
			if (sphereSphereCollision(player->getPosition(), PLAYER_COLLISION_RADIUS, (*itr)->getPosition(), BULLET_COLLISION_RADIUS))
			{
				player->dealDamage(1);

				collision = true;
			}
		}

		if ((*itr)->getLifetime() >= BULLET_LIFETIME || collision)
		{
			itr = bullets.erase(itr);

			//TODO: Show explosion
		}
		else
		{
			++itr;
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

Player * GameWorld::getPlayer()
{
	return player;
}

void GameWorld::playerFire()
{
	//If we can fire
	if (player->tryFire())
	{
		//Fire bullet
		auto facing2D = -glm::vec3(player->getRotation() * glm::vec4(0, 0, 1.f, 0.f));
		createBullet(player->getPosition() + facing2D * 0.5f + glm::vec3(0.f, 1.5f, 0.f), facing2D, true);
	}
	else
	{
		//Start reload
		player->reload();
	}
}

void GameWorld::createBullet(glm::vec3 _source, glm::vec3 _dir, bool _friendly, float _speed)
{
	assert(length2(_dir) != 0);

	PhysicsObject * bullet = new PhysicsObject(_source, glm::vec3(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS));
	bullet->setVelocity(normalize(_dir) * BULLET_MAX_SPEED);

	bullet->flag = _friendly;

	bullets.push_back(bullet);
}

bool GameWorld::isGameInProgress() const
{
	return gameInProgress;
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
