#include "Client.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

#include "Util.h"

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
			socket->sendPacket(serverAddr, Packet(PacketType::CLIENT_DISCONNECT));
		}

		//Stop receiving packets, receive thread may end without requiring socket shutdown
		active = false;

		receiveThread.detach();
	}

	void Client::processInput(const std::string & _input)
	{
		std::string messageToSend = _input;

		if (_input.size() == 0 || _input == "\n")
		{
			return;
		}

		if (_input.size() > 1)
		{
			if (_input[0] == '!' && _input[1] == '!')
			{
				messageToSend = _input.substr(1);
			}
			else if (_input[0] == '!') //Ignore double '!'s
			{
				switch (_input[1])
				{
				case '?':
					//List all commands
					std::cout << "!q - quit" << std::endl;
					std::cout << "!c - client list" << std::endl;
					std::cout << std::endl;
					break;
				case 'c':
					//send client list request
					socket->sendPacket(serverAddr, Packet(PacketType::CLIENT_LIST_REQUEST));
					break;
				case 'q':
					this->stop();
					break;
				}

				//Do not send chat message
				return;
			}
		}


		//Send a chat packet
		if (_input.size() > PACKET_SIZE - (sizeof(unsigned short) * 6))
		{
			logE("Message too long");
			return;
		}

		UDPSocketResponse r = socket->sendPacket(serverAddr, Packet(PacketType::CHAT, messageToSend));

		if (r != UDPSocketResponse::OK)
		{
			setE();
			std::cout << "Could not send chat message: code " << r << std::endl;
		}
	}

	void Client::parseClientListPacket(const Packet & _packet)
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
		std::cout << std::endl;
		if (nameList.size() > 0)
		{
			logR("Connected Clients: ");
			for (std::string name : nameList)
			{
				logR(name);
			}
		}
		else
		{
			logR("No other clients connected");
		}
		std::cout << std::endl;
	}

	bool Client::isConnectedToServer() const
	{
		return connectedToServer;
	}

	void Client::broadcastForServers()
	{
		broadcastFoundServerAddresses.clear();

		socket->enableBroadcast();


		sockaddr_in broadcastAddr;
		ZeroMemory(&broadcastAddr, sizeof(broadcastAddr));
		broadcastAddr.sin_family = AF_INET;
		broadcastAddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);


		Packet p(PacketType::SERVER_BC_REQUEST);

		//Broadcast a packet to the range of possible server addresses
		for (int i = 0; i < NUM_BIND_ATTEMPTS; ++i)
		{
			broadcastAddr.sin_port = htons(SERVER_PORT + i);

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

				Packet p(buff);

#ifdef PROCESS_PACKETS_ON_NEW_THREADS
				std::thread packetProcessThread(&Server::processPacket, this, p, addr); //Member function so pass this as well
				packetProcessThread.detach();
#else
				processPacket(p, addr);
#endif
			}
			else if (r == UDPSocketResponse::CONNECTION_CLOSED)
			{
				logE("No server present");
			}
			else
			{
				logE("Could not receive packet");
			}

			delete[] buff;
		}
	}

	void Client::processPacket(Packet _p, const sockaddr_in _addr)
	{
		if (_p.type == PacketType::KEEP_ALIVE)
		{
			socket->sendPacket(serverAddr, Packet(PacketType::KEEP_ALIVE));
			return; //Skip formatting things
		}

		GotoXY(0, GetConsoleCursorY());

		//Process packet
		switch (_p.type)
		{
		case PacketType::SERVER_BC_RESPONSE:
			broadcastFoundServerAddresses.push_back(_addr);
			break;
		case PacketType::JOIN_SERVER_ACCEPTED:
			logR("Successfully joined server");
			connectedToServer = true;
			break;
		case PacketType::JOIN_SERVER_DENIED:
			logR("Username already in use");
			connectedToServer = false;
			break;
		case PacketType::PLEASE_JOIN:
			if (connectedToServer)
			{
				//We have been removed from server
				logE("Connection was dropped, please reconnect");
			}
			else
			{
				logE("Please connect to the server");
			}

			connectedToServer = false;
			break;
		}

		if (connectedToServer) //If officially connected
		{
			switch (_p.type)
			{
			case PacketType::CHAT:
			{
				//Find color
				Color fore = static_cast<Color>((_p.data[0] % 6) + 9);

				log(_p.data, fore);
			}
			break;
			case PacketType::NEW_CLIENT:
				setR();
				std::cout << _p.data << " has joined the server!" << std::endl;
				break;
			case PacketType::CLIENT_DISCONNECT:
				setR();
				std::cout << _p.data << " has left the server" << std::endl;
				break;
			case PacketType::CLIENT_LIST:
				parseClientListPacket(_p);
				break;
			}
		}

		//Reset for command input
		if (connectedToServer)
		{
			ResetTextColor();
			std::cout << ">>";
		}
	}

	bool Client::connect(sockaddr_in _serverAddr, const std::string & _clientName)
	{
		serverAddr = _serverAddr;

		//Send a join packet
		UDPSocketResponse r = socket->sendPacket(_serverAddr, Packet(PacketType::JOIN_SERVER, _clientName));

		//If join packet unsuccessful, return false
		if (r != UDPSocketResponse::OK)
		{
			return false;
		}

		//Wait for a period, then check if connected (received a response)
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (connectedToServer == false)
		{
			return false;
		}

		return true;
	}
}