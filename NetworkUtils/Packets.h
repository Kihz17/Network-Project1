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
		PacketJoinRoom()
		{
			header.packetType = 2;
		}

		PacketHeader header;
		int roomNameLength;
		std::string roomName;
		int nameLength;
		std::string name;
		
		size_t GetSize()
		{
			return sizeof(PacketHeader)+ roomNameLength + nameLength ;
		}
	};

	struct PacketLeaveRoom
	{
		PacketLeaveRoom()
		{
			header.packetType = 3;
		}
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
		PacketSendMessage()
		{
			header.packetType = 0;
		}

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
		PacketReceiveMessage()
		{
			header.packetType = 1;
		}

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