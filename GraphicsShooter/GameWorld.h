#pragma once

#include <queue>
#include <mutex>

#include "JNetwork\NetworkEntity.h"
#include "JNetwork\UDPSocket.h"

#include "Object.h"
#include "PhysicsObject.h"
#include "Player.h"

#define ENEMY1_MAX_SPEED 5.f
#define BULLET_MAX_SPEED 75.f
#define POWERUP_MAX_SPEED 2.f

#define BULLET_LIFETIME 5.f

#define BULLET_RADIUS 0.4f
#define POWERUP_RADIUS 1.f

#define ENEMY_COLLISION_RADIUS 3.f
#define PLAYER_COLLISION_RADIUS 4.f
#define BULLET_COLLISION_RADIUS BULLET_RADIUS
#define POWERUP_COLLISION_RADIUS POWERUP_RADIUS

#define POWERUP_HEALTH_REGEN_AMOUNT 25
#define POWERUP_INFINITE_AMMO_DURATION 10.f
#define POWERUP_SHIELD_DURATION 10.f

#define SHOOTER_STOPPING_DISTANCE 30.f
#define FIGHTER_DPS 5.f


enum PowerupType
{
	HEALTH = 0,
	INFINITE_AMMO,
	SHIELD,
	POWERUPTYPE_NUM_ITEMS
};

enum EnemyType
{
	SHOOTER,
	FIGHTER,
	TURRET,
	ENEMYTYPE_NUM_ITEMS
};

class GameWorld
{
private:
	/*
	bool isServer;*/
/*
	JNetwork::INetworkEntity * networkEntity;*/
/*
	std::mutex clientPacketQueueMutex;
	std::queue<std::pair<JNetwork::JNetworkPacket, sockaddr_in>> clientPacketQueue;*/

	std::vector<Object *> enemies;

	std::vector<PhysicsObject *> bullets;

	std::vector<PhysicsObject *> powerups;

	Player * player;

	bool gameInProgress = false;

	int stage = 0;

public:
	GameWorld(/*bool _isServer*/);
	~GameWorld();

	void update(float _dt);

	std::vector<Object *> getEnemies();

	std::vector<PhysicsObject *> getBullets();

	std::vector<PhysicsObject *> getPowerups();

	Player * getPlayer();

	void playerFire();

	void createBullet(glm::vec3 _source, glm::vec3 _dir, bool _friendly, float _speed = BULLET_MAX_SPEED);

	bool isGameInProgress() const;

	void nextLevel();

	////Packet processing step 1 (pool packets if client / directly process if server)
	//void onReceivePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr);

	////Packet processing step 2
	//void parsePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr);
/*
	void startNetwork();
	void stopNetwork();


	bool getIsServer() const;

	bool clientConnect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout);

	JNetwork::INetworkEntity * getNetworkEntity();*/
};

