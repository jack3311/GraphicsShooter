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

		bool addClient(ClientInfo _clientInfo);
		void removeClient(const std::string & _name);
		bool clientConnected(const sockaddr_in & _addr);
		bool clientConnected(const std::string & _addrString);

		virtual void receiveThreadEntry();

		void processPacket(JNetworkPacket & _p, const sockaddr_in _addr);

		void sendClientList(const sockaddr_in & _addr);

		void keepAliveThreadEntry();

	public:
		Server(std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc, std::string _name);
		virtual ~Server();

		virtual void start();
		virtual void stop();


		void sendToAll(const JNetworkPacket & _p);
		void Server::sendToAllExcept(const JNetworkPacket & _p, const std::string & _name);

		const std::map<std::string, ClientInfo> & getConnectedClients() const;

		void setDenyConnections(bool _value);
	};
}