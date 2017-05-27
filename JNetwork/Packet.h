#pragma once

#include <string>

namespace JNetwork
{
	#define PACKET_SIZE 64

	enum PacketType : unsigned short
	{
		JOIN_SERVER,
		JOIN_SERVER_ACCEPTED,
		JOIN_SERVER_DENIED,
		CHAT,
		NEW_CLIENT,
		CLIENT_DISCONNECT,
		CLIENT_LIST,
		CLIENT_LIST_REQUEST,
		PLEASE_JOIN,
		SERVER_BC_REQUEST,
		SERVER_BC_RESPONSE,
		KEEP_ALIVE,
		PACKETTYPE_NUM_ITEMS
	};

	class Packet
	{
	public:
		Packet();
		Packet(const char * _netData);
		Packet(PacketType _type, const std::string & _data);
		Packet(PacketType _type);

		PacketType type;
		std::string data;

		void serialize(char * _netData) const;
	};
}