#include <WS2tcpip.h>
#include <WinSock2.h>

#include "UDPSocket.h"

#include "Util.h"

namespace JNetwork
{
	UDPSocket::UDPSocket()
	{
		m_hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_hSocket == INVALID_SOCKET)
		{
			int err = WSAGetLastError();
			//std::cout << "Error creating socket: code " << err << std::endl;
			return;
		}

		m_socketAddr.sin_family = AF_INET;
		m_socketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	UDPSocket::~UDPSocket()
	{
		closesocket(m_hSocket);
	}

	bool UDPSocket::initialise(unsigned short _port)
	{
		m_socketAddr.sin_port = htons(_port);

		int result = bind(m_hSocket, reinterpret_cast<sockaddr *>(&m_socketAddr), sizeof(m_socketAddr));

		return result == 0;
	}

	bool UDPSocket::isValidSocket() const
	{
		return m_hSocket != INVALID_SOCKET;
	}

	UDPSocketResponse UDPSocket::sendPacket(const sockaddr_in & _where, const JNetworkPacket & _packet)
	{
		char * data = new char[PACKET_SIZE];
		_packet.serialize(data);

		return sendData(_where, data);
	}

	UDPSocketResponse UDPSocket::sendData(const sockaddr_in & _where, const char * _data)
	{
		int size = static_cast<int>(strlen(_data) + 1);

		int bytesSent = sendto(m_hSocket, _data, size, 0, reinterpret_cast<const sockaddr *>(&_where), sizeof(_where));

		if (bytesSent != size)
		{
			int error = WSAGetLastError();
			//setE();
			//std::cout << "MISC SEND ERROR: " << error << std::endl;

			return UDPSocketResponse::SEND_ERROR;
		}

		return UDPSocketResponse::OK;
	}

	UDPSocketResponse UDPSocket::receivePacket(sockaddr_in & _sourceOut, JNetworkPacket & _packetOut)
	{
		char * buff = new char[PACKET_SIZE];
		memset(buff, 0, PACKET_SIZE);

		UDPSocketResponse r = receiveData(_sourceOut, buff, PACKET_SIZE);

		if (r == UDPSocketResponse::OK)
		{
			_packetOut.deserialize(buff);
		}

		delete[] buff;

		return r;
	}

	UDPSocketResponse UDPSocket::receiveData(sockaddr_in & _sourceOut, char * _dataOut, size_t _size)
	{
		int sourceSize = sizeof(_sourceOut);

		char * buffer = new char[_size];

		int bytesReceived = recvfrom(m_hSocket, buffer, _size, 0, reinterpret_cast<sockaddr *>(&_sourceOut), &sourceSize);

		if (bytesReceived < 0)
		{
			delete[] buffer;

			//Error check

			int error = WSAGetLastError();

			if (error == WSAECONNRESET) return UDPSocketResponse::CONNECTION_CLOSED;
			if (error == WSAETIMEDOUT) return UDPSocketResponse::TIMEOUT;

			//setE();
			//std::cout << "MISC RECEIVE ERROR: " << error << std::endl;
			return UDPSocketResponse::RECV_ERROR;
		}

		if (bytesReceived == 0)
		{
			delete[] buffer;
			return UDPSocketResponse::CONNECTION_CLOSED;
		}

		//Good data
		strcpy_s(_dataOut, strlen(buffer) + 1, buffer);

		delete[] buffer;

		return UDPSocketResponse::OK;
	}

	void UDPSocket::enableBroadcast()
	{
		char broadcast = 1;
		setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	}

	void UDPSocket::disableBroadcast()
	{
		char broadcast = 0;
		setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	}
}