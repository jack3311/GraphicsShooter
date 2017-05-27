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

		std::thread receiveThread;

		void receiveThreadEntry();
		void processPacket(Packet _p, const sockaddr_in _addr);

		std::vector<sockaddr_in> broadcastFoundServerAddresses;

	public:
		Client();
		virtual ~Client();

		virtual void start();
		virtual void stop();

		bool connect(sockaddr_in _serverAddr, const std::string & _clientName);

		virtual void processInput(const std::string & _input);

		void parseClientListPacket(const Packet & _packet);

		bool isConnectedToServer() const;

		void broadcastForServers();

		const std::vector<sockaddr_in> & getBroadcastFoundServers();
	};
}