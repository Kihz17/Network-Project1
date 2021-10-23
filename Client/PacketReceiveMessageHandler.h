#include "IPacketHandler.h"

class PacketReceiveMessageHandler : public IPackethandler
{
public:
	virtual void HandleOnClient(Client& client, const SOCKET& serverSocket);

private:
	friend class PacketManager;
	PacketReceiveMessageHandler();
};