#include "Server.h"

#include <algorithm>
#include <iostream>
#include <thread>

#include "Util.h"
#include "Packet.h"

namespace JNetwork
{
	#define PROCESS_PACKETS_ON_NEW_THREADS

	#define KEEP_ALIVE_TIME_MILLISECONDS 750

	Server::Server() : INetworkEntity::INetworkEntity(NetworkEntityType::SERVER)
	{
	}

	Server::~Server()
	{
	}

	void Server::start()
	{
		active = true;

		receiveThread = std::thread(&Server::receiveThreadEntry, this);
		keepAliveThread = std::thread(&Server::keepAliveThreadEntry, this);
	}

	void Server::stop()
	{
		//Stop receiving packets, receive thread may end without requiring socket shutdown
		active = false;

		receiveThread.detach();
		keepAliveThread.detach();
	}

	void Server::processInput(const std::string & _input)
	{
		if (_input.size() > 1)
		{
			if (_input[0] == '!') //Ignore double '!'s
			{
				switch (_input[1])
				{
				case '?':
					//List all commands
					std::cout << "!q - stop server" << std::endl;
					std::cout << "!k [name] - kick client with given name" << std::endl;
					std::cout << std::endl;
					break;
				case 'k':
					removeClient(_input.substr(3));
					sendToAll(Packet(PacketType::CLIENT_DISCONNECT, _input.substr(3)));
					//clientInfoMap.erase(clientInfoMap.begin());
					break;
				case 'q':
					this->stop();
					break;
				}

				return;
			}
		}
	}

	bool Server::addClient(ClientInfo _clientInfo)
	{
		std::string addressString = addrToString(_clientInfo.addr);

		//Check if client exists in map already
		auto existCheckItr = std::find_if(clientInfoMap.begin(), clientInfoMap.end(),
			[&_clientInfo](const std::pair<std::string, ClientInfo> & _c) {
			return _c.second.name == _clientInfo.name;
		}
		);

		if (existCheckItr != clientInfoMap.end())
		{
			//Client exists already
			return false;
		}


		//Proceed to add client

		clientInfoMap[addressString] = _clientInfo;

		return true;
	}

	void Server::removeClient(const std::string & _name)
	{
		for (auto itr = clientInfoMap.begin(); itr != clientInfoMap.end(); ++itr)
		{
			if (itr->second.name == _name)
			{
				clientInfoMap.erase(itr->first);
				//clientInfoMap.erase(itr);
				break;
			}
		}
	}

	bool Server::clientConnected(const sockaddr_in & _addr)
	{
		std::string addrString = addrToString(_addr);

		return clientConnected(addrString);
	}

	bool Server::clientConnected(const std::string & _addrString)
	{
		//Check if client exists in map already
		auto existCheckItr = std::find_if(clientInfoMap.begin(), clientInfoMap.end(),
			[&_addrString](const std::pair<std::string, ClientInfo> & _c) {
			return _c.first == _addrString;
		}
		);

		return existCheckItr != clientInfoMap.end();
	}

	void Server::receiveThreadEntry()
	{
		while (active)
		{
			sockaddr_in addr;
			Packet p;

			UDPSocketResponse r = socket->receivePacket(addr, p);

			if (r == UDPSocketResponse::OK)
			{
				//Process packet

#ifdef PROCESS_PACKETS_ON_NEW_THREADS
				std::thread packetProcessThread(&Server::processPacket, this, p, addr); //Member function so pass this as well
				packetProcessThread.detach();
#else
				processPacket(p, addr);
#endif
			}
			else/* if (r != UDPSocketResponse::CONNECTION_CLOSED)*/
			{
				std::cout << "Could not receive packet" << std::endl;
			}
		}
	}

	void Server::processPacket(Packet _p, const sockaddr_in _addr)
	{
		//Process received packets:

		if (_p.type == PacketType::JOIN_SERVER)
		{
			ClientInfo ci{ _p.data, _addr, true };
			if (addClient(ci))
			{
				socket->sendPacket(_addr, Packet(PacketType::JOIN_SERVER_ACCEPTED));

				//Send the client list
				sendClientList(_addr);

				//Send a notification that new client joined
				sendToAllExcept(Packet(PacketType::NEW_CLIENT, ci.name), ci.name);
			}
			else
			{
				socket->sendPacket(_addr, Packet(PacketType::JOIN_SERVER_DENIED));
			}

			return;
		}
		if (_p.type == PacketType::SERVER_BC_REQUEST)
		{
			socket->sendPacket(_addr, Packet(PacketType::SERVER_BC_RESPONSE));
			return;
		}



		std::string addrString = addrToString(_addr);
		if (clientConnected(addrString))
		{
			//Client is connected officially
			switch (_p.type)
			{
			case PacketType::CHAT:
			{
				//Relay message to all other clients
				auto clientInfo = clientInfoMap[addrString];

				std::string message = clientInfo.name + ": " + _p.data;
				sendToAllExcept(Packet(PacketType::CHAT, message), clientInfo.name);
			}
			break;
			case PacketType::CLIENT_LIST_REQUEST:
			{
				sendClientList(_addr);
			}
			break;
			case PacketType::CLIENT_DISCONNECT:
			{
				auto clientInfo = clientInfoMap[addrString];

				//Remove client from list

				removeClient(clientInfo.name);

				sendToAll(Packet(PacketType::CLIENT_DISCONNECT, clientInfo.name));
			}
			case PacketType::KEEP_ALIVE:
			{
				auto & clientInfo = clientInfoMap[addrString];
				clientInfo.keepAliveResponse = true;
			}
			break;
			}
		}
		else
		{
			//Client not connected, send a please join packet
			socket->sendPacket(_addr, Packet(PacketType::PLEASE_JOIN));
		}
	}

	void Server::sendToAll(const Packet & _p)
	{
		for (auto & client : clientInfoMap)
		{
			if (client.second.name != "")
			{
				sockaddr_in addr = client.second.addr;

				socket->sendPacket(addr, _p);
			}
		}
	}

	void Server::sendToAllExcept(const Packet & _p, const std::string & _name)
	{
		for (auto & client : clientInfoMap)
		{
			if (client.second.name != "")
			{
				sockaddr_in addr = client.second.addr;

				if (client.second.name != _name)
					socket->sendPacket(addr, _p);
			}
		}
	}

	void Server::sendClientList(const sockaddr_in & _addr)
	{
		std::string addrString = addrToString(_addr);
		if (clientConnected(addrString))
		{
			std::string tempClientList;
			for (auto & client : clientInfoMap)
			{
				sockaddr_in currentAddr = client.second.addr;

				if (addrString != addrToString(currentAddr))
				{
					tempClientList += client.second.name + ",";
				}
			}

			socket->sendPacket(_addr, Packet(PacketType::CLIENT_LIST, tempClientList));
		}
	}

	void Server::keepAliveThreadEntry()
	{
		std::chrono::high_resolution_clock clock;
		auto t1 = clock.now();

		while (active)
		{
			auto t2 = clock.now();

			if ((t2 - t1) >= std::chrono::milliseconds(KEEP_ALIVE_TIME_MILLISECONDS))
			{
				t1 = t2;

				//Check keep alive responses, and reset for next 'round'
				for (auto itr = clientInfoMap.begin(); itr != clientInfoMap.end(); )
				{
					if (!itr->second.keepAliveResponse)
					{
						//Remove client, and notify other users
						if (itr->second.name != "")
						{
							sendToAllExcept(Packet(PacketType::CLIENT_DISCONNECT, itr->second.name), itr->second.name);
						}

						itr = clientInfoMap.erase(itr);
					}
					else
					{
						itr->second.keepAliveResponse = false;
						++itr;
					}
				}

				//Send keep alive packets
				sendToAll(Packet(PacketType::KEEP_ALIVE));
			}

			std::this_thread::yield();
		}
	}
}