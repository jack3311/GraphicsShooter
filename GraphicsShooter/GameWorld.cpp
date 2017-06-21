//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    GameWorld.cpp
//  Description  :    Represents a game world and the relevant network entity
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "GameWorld.h"

#include <sstream>

#include "JNetwork\Server.h"
#include "JNetwork\Client.h"

#include "Logger.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "SceneGameOver.h"
#include "AssetManager.h"
#include "MessageType.h"


GameWorld::GameWorld(bool _isServer, bool _isMultiplayer, std::string _username, std::string _serverName) : isServer(_isServer), isMultiplayer(_isMultiplayer), username(_username)
{
	if (isMultiplayer)
	{
		if (_isServer)
		{
			tickRateClock = std::chrono::high_resolution_clock();
			lastNetworkUpdateTime = tickRateClock.now();

			//Use intermediary lambda function to use 'this' to call member function
			networkEntity = new JNetwork::Server([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
				this->onReceivePacket(_p, _a);
			}, _serverName);

			nextLevel();
		}
		else
		{
			//Use intermediary lambda function to use 'this' to call member function
			networkEntity = new JNetwork::Client([=](JNetwork::JNetworkPacket & _p, const sockaddr_in & _a) {
				this->onReceivePacket(_p, _a);
			});
		}
	}
	else
	{
		nextLevel();
	}

	//Add this player to the map
	players[username] = new Player(glm::vec3(0, 0, 0), glm::vec3(1.32f, 1.32f, 1.32f));

	gameInProgress = true;
}

GameWorld::~GameWorld()
{
	for (auto obj : enemies)
	{
		delete obj;
	}

	for (auto obj : bullets)
	{
		delete obj;
	}

	for (auto obj : powerups)
	{
		delete obj;
	}

	for (auto obj : players)
	{
		delete obj.second;
	}

	//Stop network entity
	if (isMultiplayer)
	{
		networkEntity->stop();

		delete networkEntity;
	}
}

void GameWorld::update(float _dt)
{
	if (isServer && isMultiplayer)
	{
		//Ensure all players are in the map
		for (auto client : dynamic_cast<JNetwork::Server*>(networkEntity)->getConnectedClients())
		{
			const auto & name = client.second.name;
			if (players[name] == nullptr)
			{
				players[name] = new Player(glm::vec3(0, 0, 0), glm::vec3(1.32f, 1.32f, 1.32f));
			}
		}

		//Check for disconnected clients
		for (auto itr = players.begin(); itr != players.end(); )
		{
			if (itr->first != username) //Check it is not server player
			{
				//Check it is in clients list
				bool match = false;
				for (auto client : dynamic_cast<JNetwork::Server*>(networkEntity)->getConnectedClients())
				{
					const auto & name = client.second.name;
					if (itr->first == name)
					{
						match = true;
					}
				}

				if (!match)
				{


					std::ostringstream oss;
					oss << MessageType::PLAYER_LEAVE;
					oss << itr->first;

					//Send player leave packet
					dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));

					delete itr->second;
					players.erase(itr++); //Need to increment before deleting old one
				}
				else
				{
					++itr;
				}
			}
			else
			{
				++itr;
			}
		}

	}

	if (isServer || !isMultiplayer)
	{
		//Check for player death
		bool isAPlayerAlive = false;
		for (auto playerp : players)
		{
			if (!playerp.second->isDead())
			{
				isAPlayerAlive = true;
			}
		}
		if (!isAPlayerAlive)
		{
			Logger::getLogger().log("All players died, stopping game");
			gameInProgress = false;
			SceneManager::getSceneManager().activate<SceneGameOver>(players[username]->getScore());

			if (isMultiplayer)
			{
				//Send game over packet
				std::ostringstream oss;
				oss << MessageType::GAME_OVER;
				dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));

				stopNetwork();
			}
		}

		//Check for victory
		if (enemies.size() == 0)
		{
			Logger::getLogger().log("All enemies defeated, proceeding to next level");
			nextLevel();
		}


		//Update players:
		for (auto player : players)
		{
			player.second->update(_dt);
		}

		//Update enemies:
		for (auto enemy : enemies)
		{
			//Enemy movement and shooting AI
			switch (enemy->flag)
			{
			case EnemyType::SHOOTER:
			{
				//Check if player exists
				const auto & findResult = players.find(enemy->playerTarget);
				if (findResult == players.end() ||
					findResult->second == nullptr ||
					findResult->second->isDead())
				{
					//Choose new target
					std::pair<std::string, Player *> random("", nullptr);

					for (auto playerp : players)
					{
						if (!playerp.second->isDead())
							random = playerp;
					}

					if (random.second == nullptr)
					{
						return;
					}

					enemy->playerTarget = random.first;
				}


				Player * player = players[enemy->playerTarget];

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

						if (isMultiplayer)
						{
							std::ostringstream oss;
							oss << MessageType::ENEMY_FIRED_BULLET;
							dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
						}
						else
						{
							AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("laser"));
						}
					}
				}
			}
			break;
			case EnemyType::FIGHTER:
			{
				//Check if player exists
				const auto & findResult = players.find(enemy->playerTarget);
				if (findResult == players.end() ||
					findResult->second == nullptr ||
					findResult->second->isDead())
				{
					//Choose new target
					std::pair<std::string, Player *> random("", nullptr);

					for (auto playerp : players)
					{
						if (!playerp.second->isDead())
							random = playerp;
					}

					if (random.second == nullptr)
					{
						return;
					}

					enemy->playerTarget = random.first;
				}
				

				Player * player = players[enemy->playerTarget];

				glm::vec3 delta = player->getPosition() - enemy->getPosition();
				glm::vec3 delScl = glm::normalize(delta) * ENEMY1_MAX_SPEED * _dt;

				if (length2(enemy->getPosition() + delScl - player->getPosition()) < powf(ENEMY_COLLISION_RADIUS + PLAYER_COLLISION_RADIUS, 2.f))
				{
					enemy->setPosition(player->getPosition() + glm::normalize(enemy->getPosition() - player->getPosition()) * (ENEMY_COLLISION_RADIUS + PLAYER_COLLISION_RADIUS));
					player->dealDamage(FIGHTER_DPS * _dt);

					//Try to play drill sound
					if (rand() % 10000 < 7500 * _dt)
					{
						if (isMultiplayer)
						{
							std::ostringstream oss;
							oss << MessageType::PLAYER_HIT_BY_DRILL;
							dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
						}
						else
						{
							AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode1"));
						}
					}
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

					//Send player hit by drill packet
					if (isMultiplayer)
					{
						std::ostringstream oss;
						oss << MessageType::ENEMY_TURRET_FIRED;
						dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
					}
					else
					{
						AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode2"));
					}
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
						delete (*itr2);
						itr2 = enemies.erase(itr2);

						const auto & bulletSource = (*itr)->bulletSource;

						Player * player = players[bulletSource];
						player->addScore(100);
						collision = true;

						Logger::getLogger().log("Enemy destroyed by player '", bulletSource, "', deleting enemy");
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
				for (auto playerp : players)
				{
					Player * player = playerp.second;

					if (sphereSphereCollision(player->getPosition(), PLAYER_COLLISION_RADIUS, (*itr)->getPosition(), BULLET_COLLISION_RADIUS))
					{
						Logger::getLogger().log("Player '", playerp.first, "' hit");
						//If the player doesn't have shield, deal damage
						if (!player->hasShield())
							player->dealDamage(2);

						//SEND BULLET HIT PACKET TO PLAY SOUND
						if (isMultiplayer)
						{
							std::ostringstream oss;
							oss << MessageType::PLAYER_HIT_BY_BULLET;
							dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
						}
						else
						{
							AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("impact"));
						}

						collision = true;
					}
				}
			}

			if ((*itr)->getLifetime() >= BULLET_LIFETIME || collision)
			{
				Logger::getLogger().log("Bullet expired or collision was detected, deleting bullet");
				delete (*itr);
				itr = bullets.erase(itr);
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

			bool collision = false; //Allow multiple players to get one powerup's effect
			for (auto playerp : players)
			{
				Player * player = playerp.second;
				if (sphereSphereCollision(player->getPosition(), PLAYER_COLLISION_RADIUS, (*itr)->getPosition(), POWERUP_COLLISION_RADIUS))
				{
					collision = true;

					Logger::getLogger().log("Player '", playerp.first, "' collided with powerup, deleting powerup and affecting player");

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

					//Player powerup receive packet to play sound
					if (isMultiplayer)
					{
						std::ostringstream oss;
						oss << MessageType::PLAYER_COLLECTED_POWERUP;
						dynamic_cast<JNetwork::Server*>(networkEntity)->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
					}
					else
					{
						AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("star"));
					}

					

					player->addScore(500);
				}
			}

			if (collision)
			{
				delete (*itr);
				itr = powerups.erase(itr);
			}
			else
			{
				++itr;
			}
		}

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

		//Client side prediction
		//Update bullets
		for (auto itr = bullets.begin(); itr != bullets.end(); ++itr)
		{
			if ((*itr) != nullptr)
				(*itr)->update(_dt);
		}
	}


	if (isMultiplayer)
	{
		//Send the game state if we need to
		auto currentTime = tickRateClock.now();
		auto diff = currentTime - lastNetworkUpdateTime;

		if (diff >= std::chrono::milliseconds(NETWORK_TICK_RATE))
		{
			sendGameState();

			lastNetworkUpdateTime = currentTime;
		}
	}
}


std::vector<Object *> GameWorld::getEnemies()
{
	return enemies;
}

std::vector<PhysicsObject *> GameWorld::getBullets()
{
	return bullets;
}

std::vector<PhysicsObject *> GameWorld::getPowerups()
{
	return powerups;
}

std::map<std::string, Player *> GameWorld::getPlayers()
{
	return players;
}

Player * GameWorld::getThisPlayer()
{
	return players[username];
}

void GameWorld::playerFire()
{
	Logger::getLogger().log("Player fired bullet");
	playerFire(username);
	AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().loadSound("shoot", "Assets/Sounds/shoot.wav"));
}

void GameWorld::playerFire(std::string _username)
{
	Player * player = players[_username];

	//Also functions as client side prediction:

	if (!isServer)
	{
		//Send fire packet
		JNetwork::Client * client = dynamic_cast<JNetwork::Client *>(networkEntity);

		std::ostringstream oss;
		oss << MessageType::PLAYER_TRY_FIRE << " ";
		oss << _username;

		client->send(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
	}

	//If the player can fire
	if (player->tryFire())
	{
		//Fire bullet
		auto facing2D = -glm::vec3(player->getRotation() * glm::vec4(0, 0, 1.f, 0.f));
		createBullet(player->getPosition() + facing2D * 7.f + glm::vec3(0.f, 2.0f, 0.f), facing2D, true, BULLET_MAX_SPEED, _username);
	}
	else
	{
		//Start reload
		player->reload();
	}
}

void GameWorld::createBullet(glm::vec3 _source, glm::vec3 _dir, bool _friendly, float _speed, std::string _user)
{
	assert(length2(_dir) != 0);

	PhysicsObject * bullet = new PhysicsObject(_source, glm::vec3(BULLET_RADIUS, BULLET_RADIUS, BULLET_RADIUS));
	bullet->setVelocity(normalize(_dir) * BULLET_MAX_SPEED);

	bullet->flag = _friendly;

	bullet->bulletSource = _user;

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

	for (auto obj : bullets)
	{
		delete obj;
	}
	bullets.clear();

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

	//Reset players
	for (auto playerp : players)
	{
		Player * player = playerp.second;
		player->setPosition(glm::vec3(0, 0, 0));
		player->setRotation(glm::mat4());
		player->setHealth(MAX_HEALTH);
		player->resetAmmo();
	}

	forcePlayerPositions = true;
}

void GameWorld::sendGameState()
{
	if (isServer)
	{
		JNetwork::Server * server = dynamic_cast<JNetwork::Server *>(networkEntity);
		const auto & clientList = server->getConnectedClients();
		
		//Send player info:
		for (auto playerp : players)
		{
			Player * player = playerp.second;

			//Object info
			{
				std::ostringstream oss;
				oss << MessageType::PLAYER_POSITION_UPDATE << " ";
				oss << playerp.first << " ";
				player->serialise(oss);

				if (forcePlayerPositions)
				{
					server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
				}
				else
				{
					//Do not send object info to the owner of the player
					server->sendToAllExcept(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()), playerp.first);
				}
			}

			//Player specific info
			{
				std::ostringstream oss;
				oss << MessageType::PLAYER_SPECIFIC_UPDATE << " ";
				oss << playerp.first << " ";
				player->serialiseSpecific(oss);

				//Send to all (score etc.)
				server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
			}
			
		}

		forcePlayerPositions = false;

		//Send enemy info:
		//Send array size
		{
			std::ostringstream oss;
			oss << MessageType::RESIZE_ENEMIES << " ";
			oss << enemies.size();
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}

		//Send positions
		for (unsigned int i = 0; i < enemies.size(); ++i)
		{
			Object * enemy = enemies[i];

			std::ostringstream oss;
			oss << MessageType::ENEMY_POSITION_UPDATE << " ";
			oss << i << " ";
			enemy->serialise(oss);
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}

		//Send bullet info:
		//Send array size
		{
			std::ostringstream oss;
			oss << MessageType::RESIZE_BULLETS << " ";
			oss << bullets.size();
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}

		//Send positions
		for (unsigned int i = 0; i < bullets.size(); ++i)
		{
			PhysicsObject * bullet = bullets[i];

			std::ostringstream oss;
			oss << MessageType::BULLET_POSITION_UPDATE << " ";
			oss << i << " ";
			bullet->serialise(oss);
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}

		//Send powerup info:
		//Send array size
		{
			std::ostringstream oss;
			oss << MessageType::RESIZE_POWERUPS << " ";
			oss << powerups.size();
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}

		//Send positions
		for (unsigned int i = 0; i < powerups.size(); ++i)
		{
			PhysicsObject * powerup = powerups[i];

			std::ostringstream oss;
			oss << MessageType::POWERUP_POSITION_UPDATE << " ";
			oss << i << " ";
			powerup->serialise(oss);
			server->sendToAll(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
		}
	}
	else
	{
		JNetwork::Client * client = dynamic_cast<JNetwork::Client *>(networkEntity);

		
		//Send player info
		{
			Player * player = players[username];

			if (player)
			{
				std::ostringstream oss;
				oss << MessageType::PLAYER_POSITION_UPDATE << " ";
				oss << username << " ";
				player->serialise(oss);

				client->send(JNetwork::JNetworkPacket(JNetwork::JNetworkPacketType::UPDATE, oss.str().c_str()));
			}
		}
	}
}

bool GameWorld::getIsMultiplayer() const
{
	return isMultiplayer;
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
	std::istringstream iss(&_p.data[1]); //Skip first byte since that is the JNetwork packet type
	unsigned int messageTypeInt;
	iss >> messageTypeInt;

	unsigned __int8 messageType = static_cast<unsigned __int8>(messageTypeInt);


	if (isServer)
	{
		//Server

		Logger::getLogger().log(_p.data);

		switch (messageType)
		{

		case MessageType::PLAYER_POSITION_UPDATE:
		{
			std::string pos;
			iss >> pos;

			if (players[pos] == nullptr)
				players[pos] = new Player(glm::vec3(), glm::vec3());

			players[pos]->deserialise(iss);
			break;
		}

		case MessageType::PLAYER_TRY_FIRE:
		{
			std::string username;
			iss >> username;

			playerFire(username);

			break;
		}

		}
	}
	else
	{
		//Client

		switch (messageType)
		{

		case MessageType::RESIZE_ENEMIES:
		{
			unsigned int size;
			iss >> size;

			if (size < enemies.size())
			{
				for (unsigned int i = size; i < enemies.size(); ++i)
				{
					delete enemies[i];
				}
			}

			enemies.resize(size);
			break;
		}			

		case MessageType::ENEMY_POSITION_UPDATE:
		{
			unsigned int index;
			iss >> index;

			if (index >= enemies.size())
			{
				enemies.resize(index + 1u);
			}

			if (enemies[index] == nullptr)
				enemies[index] = new PhysicsObject();

			enemies[index]->deserialise(iss);
			break;
		}

		case MessageType::RESIZE_BULLETS:
		{
			unsigned int size;
			iss >> size;

			if (size < bullets.size())
			{
				for (unsigned int i = size; i < bullets.size(); ++i)
				{
					delete bullets[i];
				}
			}

			bullets.resize(size);
			break;
		}

		case MessageType::BULLET_POSITION_UPDATE:
		{
			unsigned int index;
			iss >> index;

			if (index >= bullets.size())
			{
				bullets.resize(index + 1u);
			}

			if (bullets[index] == nullptr)
				bullets[index] = new PhysicsObject();

			bullets[index]->deserialise(iss);
			break;
		}

		case MessageType::RESIZE_POWERUPS:
		{
			unsigned int size;
			iss >> size;

			if (size < powerups.size())
			{
				for (unsigned int i = size; i < powerups.size(); ++i)
				{
					delete powerups[i];
				}
			}

			powerups.resize(size);
			break;
		}

		case MessageType::POWERUP_POSITION_UPDATE:
		{
			unsigned int index;
			iss >> index;

			if (index >= powerups.size())
			{
				powerups.resize(index + 1u);
			}

			if (powerups[index] == nullptr)
				powerups[index] = new PhysicsObject();

			powerups[index]->deserialise(iss);
			break;
		}

		case MessageType::PLAYER_POSITION_UPDATE:
		{
			std::string pos;
			iss >> pos;

			if (players[pos] == nullptr)
				players[pos] = new Player(glm::vec3(), glm::vec3());

			players[pos]->deserialise(iss);
			break;
		}

		case MessageType::PLAYER_SPECIFIC_UPDATE:
		{
			std::string pos;
			iss >> pos;

			if (players[pos] == nullptr)
				players[pos] = new Player(glm::vec3(), glm::vec3());

			players[pos]->deserialiseSpecific(iss);
			break;
		}

		case MessageType::PLAYER_LEAVE:
		{
			std::string pos;
			iss >> pos;

			players.erase(pos);

			break;
		}

		case MessageType::GAME_OVER:
		{
			SceneManager::getSceneManager().activate<SceneGameOver>(getThisPlayer()->getScore());
			stopNetwork();
			gameInProgress = false;
			break;
		}

		case MessageType::PLAYER_HIT_BY_BULLET:
		{
			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("impact"));
			break;
		}

		case MessageType::ENEMY_TURRET_FIRED:
		{
			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode2"));
			break;
		}

		case MessageType::PLAYER_COLLECTED_POWERUP:
		{
			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("star"));
			break;
		}

		case MessageType::ENEMY_FIRED_BULLET:
		{
			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("laser"));
			break;
		}

		case MessageType::PLAYER_HIT_BY_DRILL:
		{
			AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("explode1"));
			break;
		}



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