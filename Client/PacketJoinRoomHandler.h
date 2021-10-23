#include "IPacketHandler.h"

class PacketJoinRoomHandler : public IPackethandler
{
public:
	virtual void HandleOnClient(Client& client, const SOCKET& serverSocket);
		
private:
	friend class PacketManager;
	PacketJoinRoomHandler();
};