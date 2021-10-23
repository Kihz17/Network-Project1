#pragma once

#include "Buffer.h"
#include "Server.h"

class IPackethandler
{
public:
	virtual void HandleOnServer(Server& server, Client* sender) = 0;
};

