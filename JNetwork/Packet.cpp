//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Packet.cpp
// Description	: Functionality for packet creation etc.
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#include "Packet.h"

#include <iostream>
#include <string>
#include <sstream>

namespace JNetwork
{
	JNetworkPacket::JNetworkPacket() : data(nullptr)
	{
	}

	JNetworkPacket::JNetworkPacket(const char * _netData)
	{
		//Read packet type
		type = static_cast<JNetworkPacketType>(_netData[0]);

		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);

		memcpy_s(data, PACKET_SIZE, _netData, PACKET_SIZE);
	}

	//Takes in a type, and data (EXCLUDING TYPE)
	JNetworkPacket::JNetworkPacket(JNetworkPacketType _type, const char * _data) : type(_type)
	{
		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);

		data[0] = _type;

		strcpy_s(data + 1u, PACKET_CONTENT_SIZE, _data);
	}

	JNetworkPacket::JNetworkPacket(JNetworkPacketType _type) : type(_type)
	{
		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);
		data[0] = _type;
	}

	JNetworkPacket::JNetworkPacket(const JNetworkPacket & _other)
	{
		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);
		memcpy_s(data, PACKET_SIZE, _other.data, PACKET_SIZE);

		type = _other.type;
	}

	JNetworkPacket::~JNetworkPacket()
	{
		if (data != nullptr)
			delete[] data;
	}

	void JNetworkPacket::serialize(char * _netData) const
	{
		memcpy_s(_netData, PACKET_SIZE, data, PACKET_SIZE);
	}

	void JNetworkPacket::deserialize(char * _netData)
	{
		if (data)
			delete[] data;

		//Read packet type
		type = static_cast<JNetworkPacketType>(_netData[0]);

		data = new char[PACKET_SIZE];
		memset(data, 0, PACKET_SIZE);

		memcpy_s(data, PACKET_SIZE, _netData, PACKET_SIZE);
	}
}