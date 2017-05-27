#include "NetworkEntity.h"

namespace JNetwork
{
	INetworkEntity::INetworkEntity(NetworkEntityType _type) : type(_type), active(false)
	{
		socket = new UDPSocket();
	}

	INetworkEntity::~INetworkEntity()
	{
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