#include "Packet.h"

#include <sstream>
#include <string>

namespace JNetwork
{
	Packet::Packet()
	{
	}

	Packet::Packet(const char * _netData)
	{
		std::string temp(_netData);
		std::istringstream iss(temp);

		//Read packet type
		unsigned short t;
		iss >> t;
		type = static_cast<PacketType>(t);

		iss.seekg(1, std::ios_base::cur); //Skip the space

										  //Read packet content
		std::getline(iss, data);
	}

	Packet::Packet(PacketType _type, const std::string & _data) : type(_type), data(_data)
	{
	}

	Packet::Packet(PacketType _type) : type(_type), data("")
	{
	}

	void Packet::serialize(char * _netData) const
	{
		std::ostringstream oss;
		oss << static_cast<unsigned short>(type);
		oss << " ";
		oss << data;

		std::string strPacket = oss.str();
		const char * cPacket = strPacket.c_str();

		strcpy_s(_netData, strlen(cPacket) + 1, cPacket);
	}
}