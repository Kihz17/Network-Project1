#pragma once

#include "PacketHandler.h"

class PacketLeaveRoomHandler : public IPackethandler
{
public:
	virtual void HandleOnServer(Server& server, Client* sender);

private:
	friend class PacketManager;
	PacketLeaveRoomHandler();

};


