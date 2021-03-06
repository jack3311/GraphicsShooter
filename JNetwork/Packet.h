//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Packet.h
// Description	: Functionality for packet creation etc.
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <string>

#define PACKET_HEADER_SIZE sizeof(JNetworkPacketType)
#define PACKET_SIZE 300 //bytes
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

		/// 
		/// Packet:
		/// Creates (Deserializes) a packet from data send over a socket
		/// _netData - the data to deserialize
		/// 
		JNetworkPacket(const char * _netData);

		/// 
		/// Packet:
		/// Creates a packet with a given type and with given data
		/// _type - the packet type
		/// _data - the data to create the packet with
		/// 
		JNetworkPacket(JNetworkPacketType _type, const char * _data);

		/// 
		/// Packet:
		/// Creates an empty packet with a given type
		/// _type - the packet type
		/// 
		JNetworkPacket(JNetworkPacketType _type);

		JNetworkPacket(const JNetworkPacket & _other);
		~JNetworkPacket();

		JNetworkPacketType type;
		char * data;

		/// 
		/// serialize:
		/// Creates data to send over a socket from a packet
		/// _netData - the data to fill
		/// 
		void serialize(char * _netData) const;
		void deserialize(char * _netData);
	};
}