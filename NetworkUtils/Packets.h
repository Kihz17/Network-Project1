#pragma once

#include <string>

namespace netutils
{

	// 0 == Send Message
	// 1 == Receive Message
	struct PacketHeader
	{
		int packetType;
	};

	struct PacketJoinRoom
	{
		PacketHeader header;
		std::string roomName;
		std::string name;
	};

	struct PacketLeaveRoom
	{
		PacketHeader header;
		std::string name;
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