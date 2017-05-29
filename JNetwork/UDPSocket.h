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

		bool initialise(unsigned short _port);
		bool isValidSocket() const;


		UDPSocketResponse sendPacket(const sockaddr_in & _where, const JNetworkPacket & _packet);
		UDPSocketResponse sendData(const sockaddr_in & _where, const char * _data);
		UDPSocketResponse receivePacket(sockaddr_in & _sourceOut, JNetworkPacket & _packetOut);
		UDPSocketResponse receiveData(sockaddr_in & _sourceOut, char * _dataOut, size_t _size);

		void enableBroadcast();
		void disableBroadcast();
	};
}