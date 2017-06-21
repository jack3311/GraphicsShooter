//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Server.h
// Description	: Represents a server
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "NetworkEntity.h"
#include "Packet.h"

#include <map>
#include <thread>

namespace JNetwork
{
	class JNetworkPacket;

	struct ClientInfo
	{
		std::string name;
		sockaddr_in addr;
		bool keepAliveResponse;
	};

	class Server :
		public INetworkEntity
	{
	private:
		std::string name;

		std::map<std::string, ClientInfo> clientInfoMap;

		std::thread keepAliveThread;

		bool denyConnections = false;

		/// 
		/// addClient:
		/// Adds a client to the client info map
		/// _clientInfo - The client's information
		/// returns whether the client was added successfully
		/// 
		bool addClient(ClientInfo _clientInfo);

		/// 
		/// removeClient:
		/// Removes a client from the client info map
		/// _name - the name of the client to remove
		/// 
		void removeClient(const std::string & _name);

		/// 
		/// clientConnected:
		/// Returns whether a client with a given address is connected
		/// _addr - the address to check
		/// 
		bool clientConnected(const sockaddr_in & _addr);

		/// 
		/// clientConnected:
		/// Returns whether a client with a given address string is connected
		/// _addrString - the address string to check
		/// 
		bool clientConnected(const std::string & _addrString);

		/// 
		/// receiveThreadEntry:
		/// The entry point of the receive thread
		/// 
		virtual void receiveThreadEntry();

		/// 
		/// processPacket:
		/// Processes a packet of any type
		/// _p - the packet to process
		/// _addr - the sender's address
		/// 
		void processPacket(JNetworkPacket & _p, const sockaddr_in _addr);

		/// 
		/// sendClientList:
		/// Creates a client list packet, and sends it to a given client
		/// _addr - the client to send to
		/// 
		void sendClientList(const sockaddr_in & _addr);

		/// 
		/// keepAliveThreadEntry:
		/// The entry point of the keep alive thread
		/// 
		void keepAliveThreadEntry();

	public:
		Server(std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc, std::string _name);
		virtual ~Server();

		/// 
		/// start:
		/// Starts the server's receive and keepAlive threads
		/// 
		virtual void start();

		/// 
		/// stop:
		/// Stops the server's threads, and deactivates it
		/// 
		virtual void stop();

		/// 
		/// sendToAll:
		/// Sends a packet to all connected clients
		/// _p - the packet to send
		/// 
		void sendToAll(const JNetworkPacket & _p);

		/// 
		/// sendToAllExcept:
		/// Sends a packet to all connected clients except that with a given name
		/// _p - the packet to send
		/// _name - the username of the user to exclude
		/// 
		void Server::sendToAllExcept(const JNetworkPacket & _p, const std::string & _name);

		/// 
		/// getConnectedClients:
		/// returns all connected clients
		/// 
		const std::map<std::string, ClientInfo> & getConnectedClients() const;

		void setDenyConnections(bool _value);
	};
}