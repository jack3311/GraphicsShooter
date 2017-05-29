#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <thread>
#include <conio.h>
#include <sstream>

#include "UDPSocket.h"
#include "Packet.h"
#include "NetworkEntity.h"
#include "Server.h"
#include "Client.h"
#include "Util.h"

namespace JNetwork
{
	bool initWSA()
	{
		WSADATA wsaData;
		int error;
		if (WSAStartup(0x0202, &wsaData) != 0)
		{
			error = WSAGetLastError();
			//std::cout << "Error starting WSA: code" << error << std::endl;
			return false;
		}
		return true;
	}

	bool shutdownWSA()
	{
		int error;
		if (WSACleanup() != 0)
		{
			error = WSAGetLastError();
			//std::cout << "Error closing WSA: code" << error << std::endl;
			return false;
		}
		return true;
	}
}

//
//bool clientConnection(Client * _client)
//{
//	logO("Connect to a server: ");
//	//Find username
//	logO("Please enter a username: ");
//	std::string nameEntry;
//	std::getline(std::cin, nameEntry);
//
//	logO("Press any key to continue or 'm' to manually enter a server address");
//
//	sockaddr_in serverAddr;
//
//	char input = (char)_getch();
//	if (input == 'm')
//	{
//		//Manually enter address
//		logO("Please enter the IP address of the server: ");
//		std::string ip;
//		std::getline(std::cin, ip);
//
//		logO("Please enter the port of the server, or enter for default: ");
//		std::string port;
//		unsigned short portConvert;
//		std::getline(std::cin, port);
//
//		if (port.size() == 0)
//		{
//			portConvert = SERVER_PORT;
//		}
//		else
//		{
//			std::istringstream iss(port);
//			iss >> portConvert;
//		}
//
//		createAddr(serverAddr, ip, portConvert);
//	}
//	else
//	{
//		//Broadcast
//
//		logO("Broadcasting for servers...");
//		_client->broadcastForServers();
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//		auto serverList = _client->getBroadcastFoundServers();
//		if (serverList.size() == 0)
//		{
//			logO("No servers found");
//			return false;
//		}
//		else
//		{
//			ResetTextColor();
//			std::cout << "Found " << serverList.size() << " server" << (serverList.size() > 1 ? "s" : "") << ": " << std::endl;
//			for (unsigned int i = 0; i < serverList.size(); ++i)
//			{
//				std::cout << i + 1 << ": " << addrToString(serverList[i]) << std::endl;
//			}
//
//			logO("Please enter the number corresponding to the server which you wish to join");
//			
//			int chosen;
//			std::string chosenString;
//			std::getline(std::cin, chosenString);
//			std::istringstream ss(chosenString);
//			ss >> chosen;
//			--chosen;
//
//			serverAddr = serverList[chosen];
//		}
//	}
//
//
//	//Try to officially connect to server with username
//
//	if (!_client->connect(serverAddr, nameEntry))
//	{
//		logE("Could not connect to server");
//		return false;
//	}
//
//	logS("Connected to server successfully");
//
//	return true;
//}

#ifdef MAIN
int mainDONOTRUN()
{
	//Startup WSA
	if (!initWSA())
		return 0;


	INetworkEntity * entity = nullptr;

	ResetTextColor();
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "--- UDP Chat - Jack Wilson - 2017 ---" << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << std::endl;

	std::cout << "Would you like to start a client or server? (Enter 'C' or 'S') ";

	char input = (char)_getch();

	std::cout << std::endl;

	if (input == 'c' || input == 'C')
	{
		//Client
		logS("Starting CLIENT");
		
		//Create clinet object
		entity = new Client();

		//Initialise socket
		if (!entity->initialise(CLIENT_PORT))
		{
			logE("Could not start client");
			_getch();
			return 0;
		}

		//Start receiving thread
		entity->start();

		Client * entityClient = dynamic_cast<Client *>(entity);

		//Officially connect to the server
		while (!entityClient->isConnectedToServer())
			clientConnection(entityClient);
	}
	else if (input == 's' || input == 'S')
	{
		//Server
		logS("Starting SERVER");

		entity = new Server();
		if (!entity->initialise(SERVER_PORT))
		{
			logE("Could not start server");
			_getch();
			return 0;
		}

		entity->start();

		logS("Server started successfully");

	}
	else
	{
		return 0;
	}

	

	//Main loop
	
	while (entity->isActive())
	{
		ResetTextColor();
		std::cout << ">>";
		std::string s;
		std::getline(std::cin, s);

		entity->processInput(s);


		//Check for disconnect from server (if applicable)
		if (entity->getType() == NetworkEntityType::CLIENT)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			Client * entityClient = dynamic_cast<Client *>(entity);
			while (!entityClient->isConnectedToServer())
			{
				logO("Press any key to reconnect, or x to exit");
				char input = (char)_getch();

				if (input == 'x') //Quit
				{
					entity->stop();
					break;
				}

				//Try to reconnect
				clientConnection(entityClient);
			}
		}
	}

	//Close WSA
	if (!shutdownWSA())
		return 0;


	return 0;
}
#endif