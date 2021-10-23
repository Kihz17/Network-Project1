#include "PacketReceiveMessageHandler.h"

#include <iostream>
#include <string>

PacketReceiveMessageHandler::PacketReceiveMessageHandler()
{

}

void PacketReceiveMessageHandler::HandleOnClient(Client& client, const SOCKET& serverSocket)
{
	int nameLength = client.buffer.ReadInt();
	std::string name = client.buffer.ReadString(nameLength);
	int messageLength = client.buffer.ReadInt();
	std::string message = client.buffer.ReadString(messageLength);

	std::cout << "[" << name << "]: " << message << std::endl;
	std::cout << std::endl;
}