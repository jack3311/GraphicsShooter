#include "Packet.h"

#include <string>

namespace JNetwork
{
	JNetworkPacket::JNetworkPacket() : data(nullptr)
	{
	}

	JNetworkPacket::JNetworkPacket(const char * _netData)
	{
		//Read packet type
		type = static_cast<JNetworkPacketType>(_netData[0]);

		memcpy_s(data, PACKET_SIZE, _netData, PACKET_SIZE);
	}

	//Takes in a type, and data (EXCLUDING TYPE)
	JNetworkPacket::JNetworkPacket(JNetworkPacketType _type, const char * _data) : type(_type)
	{
		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);

		data[0] = _type;

		memcpy_s(data + 1u, PACKET_CONTENT_SIZE, _data, sizeof(_data));
	}

	JNetworkPacket::JNetworkPacket(JNetworkPacketType _type) : type(_type)
	{
		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);
	}

	JNetworkPacket::~JNetworkPacket()
	{
		delete[] data;
	}

	void JNetworkPacket::serialize(char * _netData) const
	{
		memcpy_s(_netData, sizeof(_netData), data, PACKET_SIZE);
	}
}