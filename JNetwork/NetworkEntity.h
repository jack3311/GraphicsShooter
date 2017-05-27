#pragma once

#include "UDPSocket.h"



namespace JNetwork
{
	#define CLIENT_PORT 5550
	#define SERVER_PORT 6660

	#define NUM_BIND_ATTEMPTS 10

	enum NetworkEntityType
	{
		CLIENT,
		SERVER,
		NETWORKENTITYTYPE_NUM_ITEMS
	};

	class INetworkEntity
	{
	private:
		NetworkEntityType type;

	protected:
		UDPSocket * socket;

		bool active;

	public:
		INetworkEntity(NetworkEntityType _type);
		~INetworkEntity();

		virtual void start() = 0;
		virtual void stop() = 0;

		bool initialise(unsigned short _port);

		NetworkEntityType getType() const;

		virtual void processInput(const std::string & _input) = 0;

		bool isActive() const;
	};
}