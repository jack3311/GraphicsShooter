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
		inet_ntop(AF_INET, &_sockAddress.sin_addr, _pcAddress, INET_ADDRSTRLEN);

		std::string _strAddress = _pcAddress;
		std::string _strPort = std::to_string(ntohs(_sockAddress.sin_port));
		std::string _strAddressPort = _strAddress + ':' + _strPort;

		return _strAddressPort;
	}
}