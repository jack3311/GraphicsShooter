#pragma once

#include <string>
#include <WS2tcpip.h>

namespace JNetwork
{
	void createAddr(sockaddr_in & _addr, std::string _ip, unsigned short _port);

	std::string addrToString(const sockaddr_in & _sockAddress);

	bool initWSA();

	bool shutdownWSA();
}