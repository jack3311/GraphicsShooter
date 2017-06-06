#pragma once

#include <queue>
#include <mutex>

#include "JNetwork\NetworkEntity.h"
#include "JNetwork\UDPSocket.h"

#include "Object.h"
#include "PhysicsObject.h"

#define ENEMY1_MAX_SPEED 0.f//5.f
#define BULLET_MAX_SPEED 0.f//40.f
#define RELOAD_TIME 2.f
#define CLIP_SIZE 10

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

	Object * player;

	unsigned int playerAmmo = CLIP_SIZE;
	float timeSinceReloadStart = 0.f;

public:
	GameWorld(/*bool _isServer*/);
	~GameWorld();

	void update(float _dt);

	std::vector<Object *> getEnemies();

	std::vector<PhysicsObject *> getBullets();

	Object * getPlayer();

	void playerFire();

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

