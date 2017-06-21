//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Client.h
// Description	: Represents a client
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "NetworkEntity.h"

#include <thread>
#include <vector>

namespace JNetwork
{
	class Client :
		public INetworkEntity
	{
	private:
		bool connectedToServer;

		sockaddr_in serverAddr;

		std::vector<std::pair<sockaddr_in, std::string>> broadcastFoundServerAddresses;

		/// 
		/// receiveThreadEntry:
		/// The entry point for the receive thread
		/// 
		void receiveThreadEntry();

		/// 
		/// processPacket:
		/// Processes a packet of any type
		/// _p - the packet to process
		/// _addr - the address of the packet sender
		/// 
		void processPacket(JNetworkPacket _p, const sockaddr_in _addr);

	public:
		Client(std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc);
		virtual ~Client();

		/// 
		/// start:
		/// Starts the client's receive thread, and sets it to active
		/// 
		virtual void start();

		/// 
		/// stop:
		/// Stops the client, and disconnects from server
		/// 
		virtual void stop();

		/// 
		/// connect:
		/// Connects the client to a given server, with a given name
		/// _serverAddr - the address of the server to connect to
		/// _clientName - the username to connect with
		/// _timeout - the timeout to connect with
		/// returns whether the connection was successful
		/// 
		bool connect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout = 500u);

		/// 
		/// parseClientListPacket:
		/// Parses a client list packet, printing the active clients
		/// _packet - the client list packet
		/// 
		void parseClientListPacket(const JNetworkPacket & _packet);

		/// 
		/// isConnectedToServer:
		/// Returns whether the client is connected to a server
		/// 
		bool isConnectedToServer() const;

		/// 
		/// broadcastForServers:
		/// Sends broadcast packet to find online servers
		/// _startPort - the port to start broadcasting on
		/// 
		void broadcastForServers(unsigned int _startPort);

		/// 
		/// getBroadcastFoundServers:
		/// Returns all of the servers who have responded to the broadcast message
		/// 
		const std::vector<std::pair<sockaddr_in, std::string>> & getBroadcastFoundServers();

		void send(const JNetworkPacket & _p);
	};
}