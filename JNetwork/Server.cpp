#include "Server.h"

#include <algorithm>
#include <thread>

#include "Util.h"
#include "Packet.h"

namespace JNetwork
{
	#define PROCESS_PACKETS_ON_NEW_THREAD
	#define KEEP_ALIVE_TIME_MILLISECONDS 750

	Server::Server(std::function<void(JNetworkPacket &, const sockaddr_in &)> _receivePacketGameFunc) : INetworkEntity::INetworkEntity(NetworkEntityType::SERVER, _receivePacketGameFunc)
	{
	}

	Server::~Server()
	{
	}

	void Server::start()
	{
		INetworkEntity::start();

		keepAliveThread = std::thread(&Server::keepAliveThreadEntry, this);
	}

	void Server::stop()
	{
		INetworkEntity::stop();

		keepAliveThread.detach();
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
			JNetworkPacket p;

			UDPSocketResponse r = socket->receivePacket(addr, p);

			if (r == UDPSocketResponse::OK)
			{
				processPacket(p, addr);
			}
			else /*if (r != UDPSocketResponse::CONNECTION_CLOSED)*/
			{
				//std::cout << "Could not receive packet" << std::endl;
			}
		}
	}

	void Server::processPacket(JNetworkPacket & _p, const sockaddr_in _addr)
	{
		//Process received packets:

		if (_p.type == JNetworkPacketType::JOIN_SERVER)
		{
			ClientInfo ci{ _p.data, _addr, true };
			if (addClient(ci))
			{
				socket->sendPacket(_addr, JNetworkPacket(JNetworkPacketType::JOIN_SERVER_ACCEPTED));

				//Send the client list
				sendClientList(_addr);

				//Send a notification that new client joined
				sendToAllExcept(JNetworkPacket(JNetworkPacketType::NEW_CLIENT, ci.name.c_str()), ci.name);
			}
			else
			{
				socket->sendPacket(_addr, JNetworkPacket(JNetworkPacketType::JOIN_SERVER_DENIED));
			}

			return;
		}
		if (_p.type == JNetworkPacketType::SERVER_BC_REQUEST)
		{
			socket->sendPacket(_addr, JNetworkPacket(JNetworkPacketType::SERVER_BC_RESPONSE));
			return;
		}


		std::string addrString = addrToString(_addr);
		if (clientConnected(addrString))
		{
			//Client is connected officially
			switch (_p.type)
			{
			case JNetworkPacketType::CLIENT_LIST_REQUEST:
			{
				sendClientList(_addr);
			}
			break;
			case JNetworkPacketType::CLIENT_DISCONNECT:
			{
				auto clientInfo = clientInfoMap[addrString];

				//Remove client from list

				removeClient(clientInfo.name);

				sendToAll(JNetworkPacket(JNetworkPacketType::CLIENT_DISCONNECT, clientInfo.name.c_str()));
			}
			break;
			case JNetworkPacketType::KEEP_ALIVE:
			{
				auto & clientInfo = clientInfoMap[addrString];
				clientInfo.keepAliveResponse = true;
			}
			break;
			case JNetworkPacketType::UPDATE:
			{
				receivePacketGameFunc(_p, _addr);
			}
			break;
			}
		}
		else
		{
			//Client not connected, send a please join packet
			socket->sendPacket(_addr, JNetworkPacket(JNetworkPacketType::PLEASE_JOIN));
		}
	}

	void Server::sendToAll(const JNetworkPacket & _p)
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

	void Server::sendToAllExcept(const JNetworkPacket & _p, const std::string & _name)
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

			socket->sendPacket(_addr, JNetworkPacket(JNetworkPacketType::CLIENT_LIST, tempClientList.c_str()));
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
							sendToAllExcept(JNetworkPacket(JNetworkPacketType::CLIENT_DISCONNECT, itr->second.name.c_str()), itr->second.name);
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
				sendToAll(JNetworkPacket(JNetworkPacketType::KEEP_ALIVE));
			}

			std::this_thread::yield();
		}
	}
}