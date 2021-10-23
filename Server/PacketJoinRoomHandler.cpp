#include "PacketJoinRoomHandler.h"

PacketJoinRoomHandler::PacketJoinRoomHandler()
{

}

void PacketJoinRoomHandler::HandleOnServer(Server& server, Client* sender) {

	int roomNameLength = sender->buffer.ReadInt();
	std::string roomName = sender->buffer.ReadString(roomNameLength); // read room name
	int nameLength = sender->buffer.ReadInt();
	std::string name = sender->buffer.ReadString(nameLength);

	netutils::PacketJoinRoom joinPacket;

	joinPacket.roomNameLength = roomNameLength;
	joinPacket.roomName = roomName;
	joinPacket.nameLength = nameLength;
	joinPacket.name = name;

	server.JoinRoom(sender, roomName, joinPacket);
}


