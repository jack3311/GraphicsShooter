//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: NetworkEntity.h
// Description	: Represents a client or server
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

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

		std::function<void(JNetworkPacket &, const sockaddr_in &)> receivePacketGameFunc;

		bool active;

	public:
		INetworkEntity(NetworkEntityType _type, std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc);
		~INetworkEntity();

		/// 
		/// start:
		/// Starts the network entity
		/// 
		virtual void start();

		/// 
		/// stop:
		/// Stops the network entity
		/// 
		virtual void stop();

		/// 
		/// receiveThreadEntry:
		/// The entry point of the receive thread
		/// 
		virtual void receiveThreadEntry() = 0;

		/// 
		/// initialise:
		/// Sets up (binds) the socket of a network entity
		/// _port - the port to bind to
		/// returns whether the bind was successful
		/// 
		bool initialise(unsigned short _port);

		/// 
		/// getType:
		/// Returns the type of the network entity
		/// 
		NetworkEntityType getType() const;

		/// 
		/// isActive:
		/// Returns whether the network entity is active (running)
		/// 
		bool isActive() const;

		/// 
		/// getSocket:
		/// Returns the underlying UDPSocket
		/// 
		const UDPSocket * getSocket() const;
	};
}