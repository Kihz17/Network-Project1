#include "PacketJoinRoomHandler.h"

#include <iostream>
#include <string>

PacketJoinRoomHandler::PacketJoinRoomHandler()
{

}

void PacketJoinRoomHandler::HandleOnClient(Client& client, const SOCKET& serverSocket)
{
	int roomNameLength = client.buffer.ReadInt();
	std::string roomName = client.buffer.ReadString(roomNameLength);
	int nameLength = client.buffer.ReadInt();
	std::string name = client.buffer.ReadString(nameLength);

	std::cout << "[" << name << "] has joined room " << roomName << "." << std::endl;
	std::cout << std::endl;
}