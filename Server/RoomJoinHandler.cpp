#include"RoomJoinHandler.h"


void JoinRoomHandler::HandleOnServer(Server& server, Client* sender) {

	int messageLength = sender->buffer.ReadInt();
	std::string message = sender->buffer.ReadString(messageLength); // read room name

	netutils::PacketJoinRoom room ;

	room.header.packetType = 2;
	room.NameLength = room.name.size();
	room.name = sender->name;
	room.roomNameLength = room.roomName.size();
	room.roomName = message;

	server.joinRoom(sender, message);

	//buffer for everyone in that room that a user has joined


	netutils::Buffer buffer(room.GetSize());

	buffer.WriteInt(room.header.packetType);
	buffer.WriteInt(room.NameLength);
	buffer.WriteString(room.name);
	buffer.WriteInt(room.roomNameLength);
	buffer.WriteString(room.roomName);

	server.BroadCastToRoom(room.roomName, buffer.data, buffer.Length());


}


