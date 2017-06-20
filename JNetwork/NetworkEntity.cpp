#include "NetworkEntity.h"

#include <thread>

namespace JNetwork
{
	INetworkEntity::INetworkEntity(NetworkEntityType _type, std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc) : type(_type), active(false), receivePacketGameFunc(_receivePacketGameFunc)
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
		receiveThread.detach();
		active = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
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

	const UDPSocket * INetworkEntity::getSocket() const
	{
		return socket;
	}
}