#pragma once

#include <string>

namespace netutils
{

	// 0 == Send Message
	// 1 == Receive Message
	// 2 == join Room 
	// 3 = leave Room
	struct PacketHeader
	{
		int packetType;
	};

	struct PacketJoinRoom
	{
		PacketHeader header;
		int roomNameLength;
		std::string roomName;
		int NameLength;
		std::string name;
		
		size_t GetSize()
		{
			return sizeof(PacketHeader)+ roomNameLength + NameLength ;
		}
	};

	struct PacketLeaveRoom
	{
		PacketHeader header;
		int roomNameLength;
		std::string roomName;
		int namelength;
		std::string name;

		size_t GetSize()
		{
			return sizeof(PacketHeader) + roomNameLength + namelength;
		}

	};

	struct PacketSendMessage
	{
		PacketHeader header;

		int messageLength;
		std::string message;

		size_t GetSize()
		{
			return sizeof(PacketHeader) + messageLength;
		}
	};

	struct PacketReceiveMessage
	{
		PacketHeader header;

		int nameLength;
		std::string senderName;

		int messageLength;
		std::string message;

		size_t GetSize()
		{
			return sizeof(PacketHeader) + nameLength + messageLength;
		}
	};
}