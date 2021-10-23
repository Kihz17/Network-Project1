#include "PacketSendMessageHandler.h"

#include "Packets.h"

PacketSendMessageHandler::PacketSendMessageHandler()
{

}

void PacketSendMessageHandler::HandleOnServer(Server& server, Client* sender)
{
	int messageLength = sender->buffer.ReadInt();
	std::string message = sender->buffer.ReadString(messageLength);

	netutils::PacketReceiveMessage receivePacket;
	receivePacket.nameLength = sender->name.size();
	receivePacket.senderName = sender->name;
	receivePacket.messageLength = messageLength;
	receivePacket.message = message;

	netutils::Buffer buffer(receivePacket.GetSize());
	buffer.WriteInt(receivePacket.header.packetType);
	buffer.WriteInt(receivePacket.nameLength);
	buffer.WriteString(receivePacket.senderName);
	buffer.WriteInt(receivePacket.messageLength);
	buffer.WriteString(receivePacket.message);

	server.BroadcastToRoomExcludeClient(server.FindClientRoom(sender), sender, buffer.data, buffer.Length()); // Broadcast message to all clients except for the sender
}


