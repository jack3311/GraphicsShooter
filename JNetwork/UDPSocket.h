//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: UDPSocket.h
// Description	: Functionality for managing a socket
// Author		: Jack Wilson
// Mail			: jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <WS2tcpip.h>

#include "Packet.h"

namespace JNetwork
{
	enum UDPSocketResponse
	{
		OK,
		SEND_ERROR,
		RECV_ERROR,
		CONNECTION_CLOSED,
		TIMEOUT,
		UDPSOCKETRESPONSE_NUM_ITEMS
	};

	class UDPSocket
	{
	private:
		SOCKET m_hSocket;
		sockaddr_in m_socketAddr;

	public:
		UDPSocket();
		~UDPSocket();

		/// 
		/// initialise:
		/// Binds the socket to a given port
		/// _port - the port to bind to
		/// returns whether the bind was successful
		/// 
		bool initialise(unsigned short _port);

		/// 
		/// isValidSocket:
		/// Returns whether the socket has been created successfully
		/// 
		bool isValidSocket() const;

		/// 
		/// sendPacket:
		/// Sends a packet to a client
		/// _where - the address to send to
		/// _packet - the packet to send
		/// returns whether the send was successful
		/// 
		UDPSocketResponse sendPacket(const sockaddr_in & _where, const JNetworkPacket & _packet);

		/// 
		/// sendPacket:
		/// Sends data to a client
		/// _where - the address to send to
		/// _data - the data to send
		/// returns whether the send was successful
		/// 
		UDPSocketResponse sendData(const sockaddr_in & _where, const char * _data);

		/// 
		/// receivePacket:
		/// Receives a packet
		/// _sourceOut - set to the address of the packets sender
		/// _packetOut - set to the packet received
		/// returns whether the receive was successful
		/// 
		UDPSocketResponse receivePacket(sockaddr_in & _sourceOut, JNetworkPacket & _packetOut);

		/// 
		/// receiveData:
		/// Receives data
		/// _sourceOut - set to the address of the packets sender
		/// _dataOut - set to the data received
		/// _size - the length of data to receive
		/// returns whether the receive was successful
		/// 
		UDPSocketResponse receiveData(sockaddr_in & _sourceOut, char * _dataOut, size_t _size);

		/// 
		/// enableBroadcast:
		/// Enables the broadcast mode on the socket
		/// 
		void enableBroadcast();

		/// 
		/// disableBroadcast:
		/// Disables the broadcast mode on the socket
		/// 
		void disableBroadcast();

		/// 
		/// manualClose:
		/// Manually shuts down the socket
		/// 
		void manualClose();
	};
}