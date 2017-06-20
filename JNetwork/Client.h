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
		void processPacket(JNetworkPacket _p, const sockaddr_in _addr);

		std::vector<std::pair<sockaddr_in, std::string>> broadcastFoundServerAddresses;

	public:
		Client(std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc);
		virtual ~Client();

		virtual void start();
		virtual void stop();

		bool connect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout = 500u);

		void parseClientListPacket(const JNetworkPacket & _packet);

		bool isConnectedToServer() const;

		void broadcastForServers(unsigned int _startPort);

		const std::vector<std::pair<sockaddr_in, std::string>> & getBroadcastFoundServers();

		void send(const JNetworkPacket & _p);
	};
}