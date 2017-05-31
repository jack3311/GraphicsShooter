#pragma once

#include <string>

#define PACKET_HEADER_SIZE sizeof(JNetworkPacketType)
#define PACKET_SIZE 64 //bytes
#define PACKET_CONTENT_SIZE (PACKET_SIZE - PACKET_HEADER_SIZE)

namespace JNetwork
{
	enum JNetworkPacketType : unsigned __int8
	{
		JOIN_SERVER = 1,
		JOIN_SERVER_ACCEPTED,
		JOIN_SERVER_DENIED,
		NEW_CLIENT,
		CLIENT_DISCONNECT,
		CLIENT_LIST,
		CLIENT_LIST_REQUEST,
		PLEASE_JOIN,
		SERVER_BC_REQUEST,
		SERVER_BC_RESPONSE,
		KEEP_ALIVE,
		UPDATE, //Pass through to game parsing
		PACKETTYPE_NUM_ITEMS
	};

	class JNetworkPacket
	{
	public:
		JNetworkPacket();
		JNetworkPacket(const char * _netData);
		JNetworkPacket(JNetworkPacketType _type, const char * _data);
		JNetworkPacket(JNetworkPacketType _type);
		JNetworkPacket(const JNetworkPacket & _other);
		~JNetworkPacket();

		JNetworkPacketType type;
		char * data;

		void serialize(char * _netData) const;
		void deserialize(char * _netData);
	};
}