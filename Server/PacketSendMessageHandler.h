#pragma once 

#include "PacketHandler.h"

class PacketSendMessageHandler : public IPackethandler
{
public:
	virtual void HandleOnServer(Server& server, Client* sender);

private:
	friend class PacketManager;
	PacketSendMessageHandler();
};

