#pragma once

#include "Buffer.h"
#include "Server.h"

class IPacketHandler
{
public:
	virtual void HandleOnServer(Server& server, Client* sender) = 0;
};

