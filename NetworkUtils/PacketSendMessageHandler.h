#include "PacketHandler.h"


class PacketSendMessageHandler : public IPackethandler
{
public:
	virtual void HandleOnServer(netutils::Buffer& buffer);
};

