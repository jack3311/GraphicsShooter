#include "Util.h"

#include <iostream>

namespace JNetwork
{
	void createAddr(sockaddr_in & _addr, std::string _ip, unsigned short _port)
	{
		ZeroMemory(&_addr, sizeof(_addr));

		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(_port);

		inet_pton(AF_INET, _ip.c_str(), &_addr.sin_addr);
	}

	std::string addrToString(const sockaddr_in & _sockAddress)
	{
		//INET_ADDRSTRLEN - maximum length for IPv4 addresses
		char _pcAddress[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, (void *)&_sockAddress.sin_addr, _pcAddress, INET_ADDRSTRLEN);

		std::string _strAddress = _pcAddress;
		std::string _strPort = std::to_string(ntohs(_sockAddress.sin_port));
		std::string _strAddressPort = _strAddress + ':' + _strPort;

		return _strAddressPort;
	}

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