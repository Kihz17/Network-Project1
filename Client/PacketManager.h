#pragma once 

#include "Client.h"
#include "IPacketHandler.h"

#include <map>

class PacketManager
{
public:
	static PacketManager* GetInstance();

	void HandlePacket(Client& client, const SOCKET& serverSocket, unsigned int packetType);

	void CleanUp();
private:
	static PacketManager* instance;
	std::map<unsigned int, IPackethandler*> handlerMap;
};