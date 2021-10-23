#pragma once

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "Client.h"

class Server
{
public:
	Server(PCSTR port);
	~Server();

	bool Initialize();

	void Start();

	void BroadcastMessage(char* dataToSend, int dataLength);

	void BroadcastMessageExcludeClient(Client* exclude, char* dataToSend, int dataLength);

	void joinRoom(Client* name, std::string roomname);

	void leaveRoom(Client* name, std::string roomname);

	void BroadCastToRoom(std::string roomName, char* dataToSend, int dataLength);

private:
	void ShutDown();

	PCSTR port;
	SOCKET connectionSocket; // This socket is used to listen for incoming connections
	SOCKET acceptSocket; // Will hold a new connection

	//create a map with rooms and clients ?
	std::map<std::string, std::vector<Client*> > rooms;

	std::vector<Client*> clients; // Holds our connected clients
};