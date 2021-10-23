#include "PacketManager.h"
#include "PacketReceiveMessageHandler.h"
#include "PacketJoinRoomHandler.h"
#include "PacketLeaveRoomHandler.h"

#include <iostream>

PacketManager* PacketManager::instance = NULL;

PacketManager* PacketManager::GetInstance()
{
	if (PacketManager::instance == NULL)
	{
		PacketManager::instance = new PacketManager();
		instance->handlerMap.insert(std::make_pair(1, new PacketReceiveMessageHandler()));
		instance->handlerMap.insert(std::make_pair(2, new PacketJoinRoomHandler()));
		instance->handlerMap.insert(std::make_pair(3, new PacketLeaveRoomHandler()));
	}

	return instance;
}

void PacketManager::HandlePacket(Client& client, const SOCKET& serverSocket, unsigned int packetType)
{
	std::map<unsigned int, IPackethandler*>::iterator it = this->handlerMap.find(packetType);
	if (it != this->handlerMap.end())
	{
		it->second->HandleOnClient(client, serverSocket);
	}
	else
	{
		std::cout << "Packet handler for type " << packetType << " was not found!" << std::endl;
	}
}

void PacketManager::CleanUp()
{
	std::map<unsigned int, IPackethandler*>::iterator it;
	for (it = this->handlerMap.begin(); it != this->handlerMap.end(); it++)
	{
		delete it->second;
	}

	this->handlerMap.clear();
}