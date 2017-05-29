#include "NetworkEntity.h"

#include <thread>

namespace JNetwork
{
	INetworkEntity::INetworkEntity(NetworkEntityType _type) : type(_type), active(false)
	{
		socket = new UDPSocket();
	}

	INetworkEntity::~INetworkEntity()
	{
	}

	void INetworkEntity::start()
	{
		active = true;

		receiveThread = std::thread(&INetworkEntity::receiveThreadEntry, this);
	}

	void INetworkEntity::stop()
	{
		//Stop receiving packets, receive thread may end without requiring socket shutdown
		active = false;
		receiveThread.detach();
	}

	bool INetworkEntity::initialise(unsigned short _port)
	{
		//Try a range of ports to bind to
		for (int i = 0; i < NUM_BIND_ATTEMPTS; ++i)
		{
			if (socket->initialise(_port + i))
				return true;
		}

		return false;
	}
	NetworkEntityType INetworkEntity::getType() const
	{
		return type;
	}

	bool INetworkEntity::isActive() const
	{
		return active;
	}
}