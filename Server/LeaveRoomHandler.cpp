#include"LeaveRoomHandler.h"


void LeaveRoomHandler::HandleOnServer(Server& server, Client* sender) {

	//creating new packet type called leave
	netutils::PacketLeaveRoom leave;

	//reading msg lenght


	int messageLength = sender->buffer.ReadInt();
	std::string message = sender->buffer.ReadString(messageLength); // read room name
	int namelenght = sender->buffer.ReadInt();
	std::string name = sender->buffer.ReadString(namelenght); // read client name
	
	//setting the struct data
	leave.header.packetType = 3;
	leave.namelength = namelenght;
	leave.name = sender->name;
	leave.roomNameLength = message.size();
	leave.roomName = message;

	//calling server leave room function to remove this client
	server.leaveRoom(sender, message);


	// creating a buffer to broadcast a msg 
	netutils::Buffer buffer(leave.GetSize());

	buffer.WriteInt(leave.header.packetType);
	buffer.WriteInt(leave.namelength);
	buffer.WriteString(leave.name);
	buffer.WriteInt(leave.roomNameLength);
	buffer.WriteString(leave.roomName);

	//broadcast the msg 
	server.BroadCastToRoom(leave.roomName, buffer.data, buffer.Length());


	//server.leaveRoom(sender)
}
