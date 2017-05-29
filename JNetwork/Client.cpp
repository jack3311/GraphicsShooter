#include "Client.h"

#include <sstream>
#include <vector>
#include <iterator>

#include "Util.h"
#include "Packet.h"

namespace JNetwork
{
	Client::Client() : INetworkEntity(NetworkEntityType::CLIENT), connectedToServer(false)
	{
	}

	Client::~Client()
	{
	}

	void Client::start()
	{
		active = true;

		receiveThread = std::thread(&Client::receiveThreadEntry, this);
	}

	void Client::stop()
	{
		if (connectedToServer)
		{
			//Send quit packet
			socket->sendPacket(serverAddr, JNetworkPacketType(JNetworkPacketType::CLIENT_DISCONNECT));
		}

		//Stop receiving packets, receive thread may end without requiring socket shutdown
		active = false;

		receiveThread.detach();
	}

	void Client::parseClientListPacket(const JNetworkPacket & _packet)
	{
		std::istringstream ss(_packet.data);

		std::vector<std::string> nameList;

		while (ss.good())
		{
			std::string current;
			std::getline(ss, current, ',');
			if (current.size() != 0)
				nameList.push_back(current);
		}

		//Display clients
		//std::cout << std::endl;
		//if (nameList.size() > 0)
		//{
		//	//logR("Connected Clients: ");
		//	for (std::string name : nameList)
		//	{
		//		//logR(name);
		//	}
		//}
		//else
		//{
		//	//logR("No other clients connected");
		//}
		//std::cout << std::endl;
	}

	bool Client::isConnectedToServer() const
	{
		return connectedToServer;
	}

	void Client::broadcastForServers(unsigned int _startPort)
	{
		broadcastFoundServerAddresses.clear();

		socket->enableBroadcast();


		sockaddr_in broadcastAddr;
		ZeroMemory(&broadcastAddr, sizeof(broadcastAddr));
		broadcastAddr.sin_family = AF_INET;
		broadcastAddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);


		JNetworkPacket p(JNetworkPacketType::SERVER_BC_REQUEST);

		//Broadcast a packet to the range of possible server addresses
		for (int i = 0; i < NUM_BIND_ATTEMPTS; ++i)
		{
			broadcastAddr.sin_port = htons(_startPort + i);

			socket->sendPacket(broadcastAddr, p);
		}

		socket->disableBroadcast();
	}

	const std::vector<sockaddr_in>& Client::getBroadcastFoundServers()
	{
		return broadcastFoundServerAddresses;
	}

	void Client::receiveThreadEntry()
	{
		while (active)
		{
			sockaddr_in addr;
			char * buff = new char[PACKET_SIZE];

			UDPSocketResponse r = socket->receiveData(addr, buff, PACKET_SIZE);

			if (r == UDPSocketResponse::OK)
			{
				//Process packet

				JNetworkPacket p(buff);

#ifdef PROCESS_PACKETS_ON_NEW_THREADS
				std::thread packetProcessThread(&Server::processPacket, this, p, addr); //Member function so pass this as well
				packetProcessThread.detach();
#else
				processPacket(p, addr);
#endif
			}
			else if (r == UDPSocketResponse::CONNECTION_CLOSED)
			{
				//logE("No server present");
			}
			else
			{
				//logE("Could not receive packet");
			}

			delete[] buff;
		}
	}

	void Client::processPacket(JNetworkPacket _p, const sockaddr_in _addr)
	{
		if (_p.type == JNetworkPacketType::KEEP_ALIVE)
		{
			socket->sendPacket(serverAddr, JNetworkPacket(JNetworkPacketType::KEEP_ALIVE));
			return; //Skip formatting things
		}

		//GotoXY(0, GetConsoleCursorY());

		//Process packet
		switch (_p.type)
		{
		case JNetworkPacketType::SERVER_BC_RESPONSE:
			broadcastFoundServerAddresses.push_back(_addr);
			break;
		case JNetworkPacketType::JOIN_SERVER_ACCEPTED:
			//logR("Successfully joined server");
			connectedToServer = true;
			break;
		case JNetworkPacketType::JOIN_SERVER_DENIED:
			//logR("Username already in use");
			connectedToServer = false;
			break;
		case JNetworkPacketType::PLEASE_JOIN:
			if (connectedToServer)
			{
				//We have been removed from server
				//logE("Connection was dropped, please reconnect");
			}
			else
			{
				//logE("Please connect to the server");
			}

			connectedToServer = false;
			break;
		}

		if (connectedToServer) //If officially connected
		{
			switch (_p.type)
			{
			break;
			case JNetworkPacketType::NEW_CLIENT:
				//setR();
				//std::cout << _p.data << " has joined the server!" << std::endl;
				break;
			case JNetworkPacketType::CLIENT_DISCONNECT:
				//setR();
				//std::cout << _p.data << " has left the server" << std::endl;
				break;
			case JNetworkPacketType::CLIENT_LIST:
				parseClientListPacket(_p);
				break;
			}
		}

		//TODO: pass on packet to game
	}

	bool Client::connect(sockaddr_in _serverAddr, const std::string & _clientName, unsigned int _timeout)
	{
		serverAddr = _serverAddr;

		//Send a join packet
		UDPSocketResponse r = socket->sendPacket(_serverAddr,
			JNetworkPacket(JNetworkPacketType::JOIN_SERVER, _clientName.c_str()));

		//If join packet unsuccessful, return false
		if (r != UDPSocketResponse::OK)
		{
			return false;
		}

		//Wait for a period, then check if connected (received a response)
		std::this_thread::sleep_for(std::chrono::milliseconds(_timeout));
		if (connectedToServer == false)
		{
			return false;
		}

		return true;
	}
}