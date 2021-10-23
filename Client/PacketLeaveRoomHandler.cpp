#include "PacketLeaveRoomHandler.h"

#include <string>
#include <iostream>

PacketLeaveRoomHandler::PacketLeaveRoomHandler()
{

}

void PacketLeaveRoomHandler::HandleOnClient(Client& client, const SOCKET& serverSocket)
{
	int roomNameLength = client.buffer.ReadInt();
	std::string roomName = client.buffer.ReadString(roomNameLength);
	int nameLength = client.buffer.ReadInt();
	std::string name = client.buffer.ReadString(nameLength);

	std::cout << "[" << name << "] has left room " << roomName << "." << std::endl;
	std::cout << std::endl;
}