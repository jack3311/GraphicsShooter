#pragma once

#include <thread>

#include "UDPSocket.h"

namespace JNetwork
{
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
		std::thread receiveThread;

	protected:
		UDPSocket * socket;


		bool active;

	public:
		INetworkEntity(NetworkEntityType _type);
		~INetworkEntity();

		virtual void start();
		virtual void stop();

		virtual void receiveThreadEntry() = 0;

		bool initialise(unsigned short _port);

		NetworkEntityType getType() const;

		bool isActive() const;
	};
}