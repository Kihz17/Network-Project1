#include "IPacketHandler.h"

class PacketLeaveRoomHandler : public IPackethandler
{
public:
	virtual void HandleOnClient(Client& client, const SOCKET& serverSocket);

private:
	friend class PacketManager;
	PacketLeaveRoomHandler();
};