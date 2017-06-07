#pragma once

#include <queue>
#include <mutex>

#include "JNetwork\NetworkEntity.h"
#include "JNetwork\UDPSocket.h"

#include "Object.h"
#include "PhysicsObject.h"
#include "Player.h"

#define ENEMY1_MAX_SPEED 5.f
#define BULLET_MAX_SPEED 40.f
#define BULLET_LIFETIME 5.f
#define BULLET_RADIUS 0.4f
#define ENEMY_COLLISION_RADIUS 2.f
#define PLAYER_COLLISION_RADIUS 4.f
#define BULLET_COLLISION_RADIUS BULLET_RADIUS

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

	Player * player;

	bool gameInProgress = false;

public:
	GameWorld(/*bool _isServer*/);
	~GameWorld();

	void update(float _dt);

	std::vector<Object *> getEnemies();

	std::vector<PhysicsObject *> getBullets();

	Player * getPlayer();

	void playerFire();

	void createBullet(glm::vec3 _source, glm::vec3 _dir, bool _friendly, float _speed = BULLET_MAX_SPEED);

	bool isGameInProgress() const;

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

