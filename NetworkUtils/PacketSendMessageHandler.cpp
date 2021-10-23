#include "PacketSendMessageHandler.h"

namespace netutils
{
	void PacketSendMessageHandler::HandleOnClient(Buffer& buffer)
	{

	}

	void PacketSendMessageHandler::HandleOnServer(Buffer& buffer)
	{
		int messageLength = buffer.ReadInt();
		std::string message = buffer.ReadString(messageLength);


	}
}
