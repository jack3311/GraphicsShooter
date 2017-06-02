#pragma once

#include <queue>
#include <mutex>

#include "JNetwork\NetworkEntity.h"
#include "JNetwork\UDPSocket.h"

#include "Object.h"

class GameWorld
{
private:
	bool isServer;

	JNetwork::INetworkEntity * networkEntity;

	std::mutex clientPacketQueueMutex;
	std::queue<std::pair<JNetwork::JNetworkPacket, sockaddr_in>> clientPacketQueue;

public:
	//Test Objects TODO: Remove
	Object * testObject;



	GameWorld(bool _isServer);
	~GameWorld();

	void update(float _dt);

	//Packet processing step 1 (pool packets if client / directly process if server)
	void onReceivePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr);

	//Packet processing step 2
	void parsePacket(JNetwork::JNetworkPacket & _p, const sockaddr_in & _addr);

	void startNetwork();
	void stopNetwork();


	bool getIsServer() const;

	bool clientConnect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout);

	JNetwork::INetworkEntity * getNetworkEntity();
};

