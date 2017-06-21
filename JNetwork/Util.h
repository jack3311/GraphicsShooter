//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Util.h
// Description	: Utility Functions
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <string>
#include <WS2tcpip.h>

namespace JNetwork
{
	/// 
	/// createAddr:
	/// Creates a sockaddr_in struct from a given ip/port
	/// _addr - set to the created sockaddr_in struct
	/// _ip - the ip to use
	/// _port - the port to use
	/// 
	void createAddr(sockaddr_in & _addr, std::string _ip, unsigned short _port);

	/// 
	/// addrToString:
	/// Creates a user-readable string from a sockaddr_in struct
	/// _sockAddress - the address of the sockaddr_in struct to use
	/// returns the user-readable string
	/// 
	std::string addrToString(const sockaddr_in & _sockAddress);

	/// 
	/// initWSA:
	/// Sets up the windows sockets api
	/// returns whether the initialization was successful
	/// 
	bool initWSA();

	/// 
	/// shutdownWSA:
	/// Shuts down the windows sockets api
	/// returns whether the shutdown was successful
	/// 
	bool shutdownWSA();
}