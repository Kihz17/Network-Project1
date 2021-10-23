#pragma once

#include "PacketHandler.h"
#include "Packets.h"


class LeaveRoomHandler : public IPackethandler
{
public:
	virtual void HandleOnServer(Server& server, Client* sender);

private:

	friend class PacketManager;
	LeaveRoomHandler();

};


