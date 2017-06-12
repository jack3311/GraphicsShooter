#include "GameWorld.h"

#include "JNetwork\Server.h"
#include "JNetwork\Client.h"

#include "Logger.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "SceneGameOver.h"
#include "AssetManager.h"


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

	player = new Player(glm::vec3(0, 0, 0), glm::vec3(1.32f, 1.32f, 1.32f));

	nextLevel();

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
		Logger::getLogger().log("Player died, stopping game");
		gameInProgress = false;
		SceneManager::getSceneManager().activate<SceneGameOver>(player->getScore());
	}

	//Check for victory
	if (enemies.size() == 0)
	{
		Logger::getLogger().log("All enemies defeated, proceeding to next level");
		nextLevel();
	}



	//Update player:
	player->update(_dt);

	//Update enemies:
	for (auto enemy : enemies)
	{	
		//Enemy movement and shooting AI
		switch (enemy->flag)
		{
		case EnemyType::SHOOTER:
		{
			//Path towards player
			glm::vec3 delta = player->getPosition() - enemy->getPosition();
			glm::vec3 delScl = glm::normalize(delta) * ENEMY1_MAX_SPEED * _dt;

			if (length2(enemy->getPosition() + delScl - player->getPosition()) < SHOOTER_STOPPING_DISTANCE * SHOOTER_STOPPING_DISTANCE)
			{
				enemy->setPosition(player->getPosition() + glm::normalize(enemy->getPosition() - player->getPosition()) * SHOOTER_STOPPING_DISTANCE);
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
					AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("laser"));
				}
			}
		}
		break;
		case EnemyType::FIGHTER:
		{
			glm::vec3 delta = player->getPosition() - enemy->getPosition();
			glm::vec3 delScl = glm::normalize(delta) * ENEMY1_MAX_SPEED * _dt;

			if (length2(enemy->getPosition() + delScl - player->getPosition()) < powf(ENEMY_COLLISION_RADIUS + PLAYER_COLLISION_RADIUS, 2.f))
			{
				enemy->setPosition(player->getPosition() + glm::normalize(enemy->getPosition() - player->getPosition()) * (ENEMY_COLLISION_RADIUS + PLAYER_COLLISION_RADIUS));
				player->dealDamage(FIGHTER_DPS * _dt);
				
				//Try to play drill sound
				if (rand() % 10000 < 7500 * _dt)
					AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode1"));
			}
			else
			{
				enemy->move(delScl);
			}

			//Face towards player
			enemy->lookAt(player->getPosition());
		}
		break;
		case EnemyType::TURRET:
		{
			//Try to shoot
			if (rand() % 10000 < 1000 * _dt)
			{
				//Fire in 8 paths around the enemy
				for (float theta = 0.f; theta <= 2.f * glm::pi<float>(); theta += 2.f * glm::pi<float>() / 8.f)
				{
					glm::vec3 vel = glm::vec3(cosf(theta), 0.f, sinf(theta));
					createBullet(enemy->getPosition() + vel * 5.f + glm::vec3(0.f, (36.f / 40.f) * 4.f, 0.f), vel, false);
				}

				AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode2"));
			}
		}
		break;
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
					//Destroy the enemy
					Logger::getLogger().log("Enemy destroyed, deleting enemy");
					delete (*itr2);
					itr2 = enemies.erase(itr2);
					player->addScore(100);
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
				Logger::getLogger().log("Player hit, damaging player");
				//If the player doesn't have shield, deal damage
				if (!player->hasShield())
					player->dealDamage(2);

				AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("impact"));

				collision = true;
			}
		}

		if ((*itr)->getLifetime() >= BULLET_LIFETIME || collision)
		{
			Logger::getLogger().log("Bullet expired or collision was detected, deleting bullet");
			delete (*itr);
			itr = bullets.erase(itr);

			//TODO: Show explosion
		}
		else
		{
			++itr;
		}
	}

	//Update powerups
	for (auto itr = powerups.begin(); itr != powerups.end(); )
	{
		float lt = (*itr)->getLifetime();
		
		auto velocity = glm::vec3(
			sinf(lt / 10.f) * POWERUP_MAX_SPEED,
			cosf(lt),
			cos(lt / 10.f) * POWERUP_MAX_SPEED);

		(*itr)->setVelocity(velocity);

		(*itr)->update(_dt);

		//Check for collision with player
		if (sphereSphereCollision(player->getPosition(), PLAYER_COLLISION_RADIUS, (*itr)->getPosition(), POWERUP_COLLISION_RADIUS))
		{
			Logger::getLogger().log("Collision with powerup, deleting powerup and affecting player");

			//Process the powerup
			switch ((*itr)->flag)
			{
			case PowerupType::HEALTH:
				//Restore the player's health
				player->dealDamage(-POWERUP_HEALTH_REGEN_AMOUNT);
				break;
			case PowerupType::INFINITE_AMMO:
				//Give player infinite ammo
				player->infiniteAmmoFor(POWERUP_INFINITE_AMMO_DURATION);
				break;
			case PowerupType::SHIELD:
				//Give player shield
				player->shieldFor(POWERUP_SHIELD_DURATION);
				break;
			}

			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("star"));
			player->addScore(500);
			delete (*itr);
			itr = powerups.erase(itr);
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

std::vector<PhysicsObject*> GameWorld::getPowerups()
{
	return powerups;
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
		Logger::getLogger().log("Player fired bullet");

		//Fire bullet
		auto facing2D = -glm::vec3(player->getRotation() * glm::vec4(0, 0, 1.f, 0.f));
		createBullet(player->getPosition() + facing2D * 7.f + glm::vec3(0.f, 2.0f, 0.f), facing2D, true);

		AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().loadSound("shoot", "Assets/Sounds/shoot.wav"));
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

void GameWorld::nextLevel()
{
	++stage;

	//Clear previous level
	for (auto obj : enemies)
	{
		delete obj;
	}
	enemies.clear();

	for (auto obj : powerups)
	{
		delete obj;
	}
	powerups.clear();


	Logger::getLogger().log("Creating enemies and powerups");

	//Create enemies
	for (int i = 0; i < 15 * stage; ++i)
	{
		enemies.push_back(new Object(glm::vec3(rand() % 201 - 100, 0, rand() % 201 - 100), glm::vec3(4.f / 40.f, 4.f / 40.f, 4.f / 40.f)));
		enemies[i]->flag = rand() % EnemyType::ENEMYTYPE_NUM_ITEMS;
	}

	//Create powerups
	for (int i = 0; i < 5 + stage; ++i)
	{
		powerups.push_back(new PhysicsObject(glm::vec3(rand() % 201 - 100, 1.5f, rand() % 201 - 100), glm::vec3(POWERUP_RADIUS, POWERUP_RADIUS, POWERUP_RADIUS)));
		powerups[i]->update(static_cast<float>(rand())); //Offset lifetime
		powerups[i]->flag = rand() % PowerupType::POWERUPTYPE_NUM_ITEMS;
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
