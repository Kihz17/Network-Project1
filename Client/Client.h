#pragma once

#include "Buffer.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

class Client
{
public:
	Client(PCSTR serverIp, PCSTR port);
	~Client();

	bool Initialize();

	void Start();

	bool SendToServer(char* data, int dataLength);

	netutils::Buffer buffer;

private:
	void ShutDown();

	bool running;

	std::string currentRoom;
	std::string name;

	PCSTR serverIp;
	PCSTR serverPort;
	SOCKET serverSocket;
};