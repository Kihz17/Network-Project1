#pragma once
#pragma once

#include "Buffer.h"
#include "Client.h"

class IPackethandler
{
public:
	virtual void HandleOnClient(Client& client, const SOCKET& serverSocket) = 0;
};


